#include "db.h"

#include "sqlDefines.h"

#include <QString>
#include <QObject>
#include <QDebug>

using namespace db;

QString graph::name(int id) {
    R_PREPARE_NEW(query, " \
        SELECT name \
        FROM graphs \
        WHERE id = ? \
    ", "")
    query.addBindValue(id);
    R_EXEC(query, "")
    if (!query.next()) {
        return "";
    }
    return query.value(0).toString();
}

int graph::count(int id) {
    R_PREPARE_NEW(query, " \
        SELECT COUNT(*) \
        FROM graphNodes n, graphs g \
        WHERE g.id = ? \
          AND n.graphId = g.id \
        GROUP BY g.id \
    ", -1)
    query.addBindValue(id);
    R_EXEC(query, -1)
    if (!query.next()) {
        return -1;
    }
    return query.value(0).toInt();
}

void graph::del(int id) {
    QSqlQuery query;

    PREPARE(query, " \
        DELETE \
        FROM graphs \
        WHERE id = ? \
    ")
    query.addBindValue(id);
    EXEC(query)
    query.finish();

    PREPARE(query, " \
        DELETE \
        FROM graphNodes \
        WHERE graphId = ? \
    ")
    query.addBindValue(id);
    EXEC(query)
}

int graph::write(int id, const QString& name) {
    QSqlQuery query;

    if (id == -1) {
        R_PREPARE(query, " \
            INSERT \
            INTO graphs(name) \
            VALUES (NULLIF(:name, '')) \
        ", -1)
    } else {
        R_PREPARE(query, " \
            UPDATE graphs \
            SET name = NULLIF(:name, '') \
            WHERE id = :id \
        ", -1)
    }

    query.bindValue(":name", name);
    if (id != -1) {
        query.bindValue(":id", id);
    }

    if (!query.exec()) {
        switch(ERR_CODE(query)) {
            case SQLITE_CONSTRAINT_UNIQUE:
                throw QObject::tr("Name is not unique.");

            case SQLITE_CONSTRAINT_NOTNULL:
                throw QObject::tr("Name should not be empty.");

            default:
                LOG_FAILED_QUERY(query);
                throw 0;
        }
    }

    if (id == -1) {
        id = query.lastInsertId().toInt();
    }

    return id;

}

QList<GraphForList> graph::readForList(const QString& name) {
    R_PREPARE_NEW(query, " \
        SELECT g.id, g.name, ( \
            SELECT COUNT(*) \
            FROM graphNodes n \
            WHERE n.graphId = g.id \
        ) \
        FROM graphs g \
        WHERE g.name LIKE ('%' || ? || '%') \
        ORDER BY g.name \
    ", {})
    query.addBindValue(name);
    R_EXEC(query, {})

    QList<GraphForList> graphs;
    while (query.next()) {
        graphs.append(GraphForList{
            .id = query.value(0).toInt(),
            .name = query.value(1).toString(),
            .count = query.value(2).toInt()
        });
    }
    return graphs;
}
