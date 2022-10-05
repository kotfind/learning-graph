#include "db.h"

#include "sqlDefines.h"

#include <QVariant>

using namespace db;

ThemeEdge::ThemeEdge() : id(-1) {}

QList<ThemeEdge> themeEdge::reads(int graphId, int themeId) {
    QString queryString = " \
        WITH themeIds AS ( \
            SELECT themeId \
            FROM graphNodes \
            WHERE graphId = :graphId \
        ) \
        SELECT id, beginId, endId \
        FROM themeEdges \
        WHERE {whereSection} \
    ";

    if (themeId == -1) {
        queryString.replace("{whereSection}", " \
                beginId IN themeIds \
            AND endId IN themeIds \
        ");
    } else {
        queryString.replace("{whereSection}", " \
            ( \
                   beginId = :themeId \
               AND endId in themeIds \
            ) OR ( \
                   endId = :themeId \
               AND beginId in themeIds \
            ) \
        ");
    }

    R_PREPARE_NEW(query, queryString, {})

    query.bindValue(":graphId", graphId);
    if (themeId != -1) {
        query.bindValue(":themeId", themeId);
    }

    R_EXEC(query, {})

    QList<ThemeEdge> edges;
    while (query.next()) {
        ThemeEdge e;
        e.id = query.value(0).toInt();
        e.beginId = query.value(1).toInt();
        e.endId = query.value(2).toInt();
        edges.append(e);
    }
    return edges;
}
