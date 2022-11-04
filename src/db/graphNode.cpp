#include "db.h"
#include "sqlDefines.h"

using namespace db;

GraphNode::GraphNode() : id(-1) {}

GraphNode graphNode::read(int id) {
    PREPARE_NEW(query, "\
        SELECT graphId, themeId, x, y \
        FROM graphNodes \
        WHERE id = ? \
    ")
    query.addBindValue(id);
    EXEC(query);
    if (!query.next()) {
        return GraphNode();
    }

    GraphNode n;
    n.id = id;
    n.graphId = query.value(0).toInt();
    n.themeId = query.value(1).toInt();
    n.x = query.value(2).toDouble();
    n.y = query.value(3).toDouble();
    return n;
}

QList<GraphNode> graphNode::reads(int graphId) {
    PREPARE_NEW(query, "\
        SELECT id, graphId, themeId, x, y \
        FROM graphNodes \
        WHERE graphId = ? \
    ")
    query.addBindValue(graphId);
    EXEC(query);

    QList<GraphNode> nodes;
    while (query.next()) {
        GraphNode n;
        n.id = query.value(0).toInt();
        n.graphId = query.value(1).toInt();
        n.themeId = query.value(2).toInt();
        n.x = query.value(3).toDouble();
        n.y = query.value(4).toDouble();
        nodes.append(n);
    }
    return nodes;
}

void graphNode::move(int id, double x, double y) {
    PREPARE_NEW(query, " \
        UPDATE graphNodes \
        SET x = ?, \
            y = ? \
        WHERE id = ? \
    ")
    query.addBindValue(x);
    query.addBindValue(y);
    query.addBindValue(id);
    EXEC(query)
}

int graphNode::create(const GraphNode& n) {
    PREPARE_NEW(query, " \
        INSERT \
        INTO graphNodes(graphId, themeId, x, y) \
        VALUES (?, ?, ?, ?) \
    ")
    query.addBindValue(n.graphId);
    query.addBindValue(n.themeId);
    query.addBindValue(n.x);
    query.addBindValue(n.y);
    EXEC(query)

    return query.lastInsertId().toInt();
}

void graphNode::del(int id) {
    PREPARE_NEW(query, " \
        DELETE \
        FROM graphNodes \
        WHERE id = ? \
    ")
    query.addBindValue(id);
    EXEC(query)
}

int graphNode::themeId(int id) {
    PREPARE_NEW(query, " \
        SELECT themeId \
        FROM graphNodes \
        WHERE id = ? \
    ")
    query.addBindValue(id);
    EXEC(query)

    if (!query.next()) {
        throw 0;
    }

    return query.value(0).toInt();
}
