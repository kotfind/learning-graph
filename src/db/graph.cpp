#include "db.h"

#include "sqlDefines.h"

#include <QString>
#include <QObject>
#include <QDebug>

using namespace db;

Graph::Graph() : id(-1) {}

QString graph::name(int id) {
    PREPARE_NEW(query, " \
        SELECT name \
        FROM graphs \
        WHERE id = ? \
    ")
    query.addBindValue(id);
    EXEC(query)
    if (!query.next()) {
        throw 0;
    }
    return query.value(0).toString();
}

int graph::count(int id) {
    PREPARE_NEW(query, " \
        SELECT COUNT(*) \
        FROM graphNodes n, graphs g \
        WHERE g.id = ? \
          AND n.graphId = g.id \
        GROUP BY g.id \
    ")
    query.addBindValue(id);
    EXEC(query)
    if (!query.next()) {
        throw 0;
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

int graph::write(const Graph& g) {
    QSqlQuery query;

    int id = g.id;

    if (id == -1) {
        PREPARE(query, " \
            INSERT \
            INTO graphs(name) \
            VALUES (NULLIF(:name, '')) \
        ")
    } else {
        PREPARE(query, " \
            UPDATE graphs \
            SET name = NULLIF(:name, '') \
            WHERE id = :id \
        ")
    }

    query.bindValue(":name", g.name);
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

QList<Graph> graph::reads(const QString& name) {
    PREPARE_NEW(query, " \
        SELECT g.id, g.name, ( \
            SELECT COUNT(*) \
            FROM graphNodes n \
            WHERE n.graphId = g.id \
        ) \
        FROM graphs g \
        WHERE g.name LIKE ('%' || ? || '%') \
        ORDER BY g.name \
    ")
    query.addBindValue(name);
    EXEC(query)

    QList<Graph> graphs;
    while (query.next()) {
        Graph g;

        g.id = query.value(0).toInt();
        g.name = query.value(1).toString();
        g.count = query.value(2).toInt();

        graphs.append(g);
    }
    return graphs;
}

Graph graph::read(int id) {
    PREPARE_NEW(query, " \
        SELECT g.id, g.name, ( \
            SELECT COUNT(*) \
            FROM graphNodes n \
            WHERE n.graphId = g.id \
        ) \
        FROM graphs g \
        WHERE id = ? \
    ")
    query.addBindValue(id);
    EXEC(query);

    if (!query.next()) {
        return Graph();
    }

    Graph g;
    g.id = query.value(0).toInt();
    g.name = query.value(1).toString();
    g.count = query.value(2).toInt();
    return g;
}
