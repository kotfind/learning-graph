#include "GraphScene.h"

#include "ComboboxIdDialog.h"
#include "sqlDefines.h"
#include "GlobalSignalHandler.h"
#include "ThemeInfoDialog.h"
#include "GraphEdge.h"

#include <QDebug>
#include <QMessageBox>
#include <QHash>

GraphScene::GraphScene()
        : QGraphicsScene() {
    connect(
        this,
        &GraphScene::graphsUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::graphsUpdated
    );
}

void GraphScene::setMode(GraphEditMode mode) {
    this->mode = mode;
}

template<typename T>
T GraphScene::typedItemAt(const QPointF& pos) {
    for (auto* item : items(pos)) {
        auto* newItem = qgraphicsitem_cast<T>(item);
        if (newItem) {
            return newItem;
        }
    }
    return nullptr;
}

void GraphScene::open(int graphId) {
    this->graphId = graphId;

    clear();

    // Add Nodes
    PREPARE_NEW(query, " \
        SELECT id \
        FROM graphNodes \
        WHERE graphId = ? \
    ")
    query.addBindValue(graphId);
    LOG_EXEC(query)

    QHash<int, GraphNode*> nodes;
    while (query.next()) {
        auto nodeId = query.value(0).toInt();
        auto* node = new GraphNode(nodeId);
        nodes[nodeId] = node;
        addItem(node);
    }
    query.finish();

    // Add Edges
    LOG_PREPARE(query, " \
        WITH nodeIds AS ( \
            SELECT id \
            FROM graphNodes \
            WHERE graphId = ? \
        ) \
        SELECT id, beginId, endId \
        FROM themeEdges \
        WHERE beginId IN nodeIds \
          AND endId IN nodeIds \
    ")
    query.addBindValue(graphId);
    LOG_EXEC(query)

    while (query.next()) {
        auto* edge = new GraphEdge(
            query.value(0).toInt(),
            nodes[query.value(1).toInt()],
            nodes[query.value(2).toInt()]
        );
        addItem(edge);
    }
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    switch (mode) {
        case NEW_NODE_EDIT_MODE:
            newNode(e->scenePos());
            break;

        case EDGE_EDIT_MODE:
            pressedNode = typedItemAt<GraphNode*>(e->scenePos());
            break;

        case CURSOR_EDIT_MODE:
            QGraphicsScene::mousePressEvent(e);
            break;
    }
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    switch (mode) {
        case EDGE_EDIT_MODE:
            auto* releasedNode = typedItemAt<GraphNode*>(e->scenePos());

            if (!releasedNode || !pressedNode) {
                return;
            }

            newEdge(pressedNode, releasedNode);

            break;
    }

    QGraphicsScene::mouseReleaseEvent(e);
}

void GraphScene::newNode(const QPointF& pos) {
    ComboboxIdDialog d((QWidget*)views()[0]);
    d.setLabel(tr("Choose theme to add:"));
    d.addItem(tr("<New Node>"), -1);

    PREPARE_NEW(query, " \
        SELECT id, name, ( \
            SELECT name \
            FROM packages \
            WHERE id = packageId \
        ) \
        FROM themes \
        WHERE id NOT IN ( \
            SELECT themeId \
            FROM graphNodes \
            WHERE graphId = ? \
        ) \
        ORDER BY ( \
            SELECT name \
            FROM packages \
            WHERE id = packageId \
        ), name \
    ")
    query.addBindValue(graphId);
    LOG_EXEC(query)

    while (query.next()) {
        d.addItem(
            QString("%1 (%2)")
                .arg(query.value(1).toString())
                .arg(query.value(2).toString()),
            query.value(0).toInt()
        );
    }

    if (d.exec() == QDialog::Rejected) {
        return;
    }

    auto themeId = d.getId();

    if (themeId == -1) {
        ThemeInfoDialog d(-1, (QWidget*)views()[0]);

        if (d.exec() == QDialog::Rejected) {
            return;
        }

        themeId = d.getId();
    }

    LOG_PREPARE(query, " \
        INSERT \
        INTO graphNodes(graphId, themeId, x, y) \
        VALUES (?, ?, ?, ?) \
    ")
    query.addBindValue(graphId);
    query.addBindValue(themeId);
    query.addBindValue(pos.x());
    query.addBindValue(pos.y());
    LOG_EXEC(query)

    auto* node = new GraphNode(query.lastInsertId().toInt());
    addItem(node);

    emit graphsUpdated();
}

void GraphScene::newEdge(GraphNode* beginNode, GraphNode* endNode) {
    PREPARE_NEW(query, " \
        INSERT \
        INTO themeEdges(beginId, endId) \
        VALUES (( \
                SELECT themeId \
                FROM graphNodes \
                WHERE id = ? \
            ),( \
                SELECT themeId \
                FROM graphNodes \
                WHERE id = ? \
        ))")
    query.addBindValue(beginNode->getId());
    query.addBindValue(endNode->getId());

    query.exec();
    switch(ERR_CODE(query)) {
        case SQLITE_CONSTRAINT_UNIQUE:
            QMessageBox::critical(
                (QWidget*)views()[0],
                tr("Error"),
                tr("This edge already exists.")
            );
            return;

        case SQLITE_CONSTRAINT_CHECK:
            QMessageBox::critical(
                (QWidget*)views()[0],
                tr("Error"),
                tr("The begining and ending of the edge should be different nodes.")
            );
            return;

        default:
            LOG_FAILED_QUERY(query)
    }

    auto* edge = new GraphEdge(
        query.lastInsertId().toInt(),
        beginNode,
        endNode
    );
    addItem(edge);

}
