#include "db.h"

#include "sqlDefines.h"

#include <QString>

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
