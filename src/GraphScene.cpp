#include "GraphScene.h"

#include "ComboboxIdDialog.h"
#include "sqlDefines.h"
#include "GlobalSignalHandler.h"
#include "ThemeInfoDialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QHash>
#include <QMimeData>

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
    themeIdToNode.clear();

    // Add Nodes
    PREPARE_NEW(query, " \
        SELECT themeId, id \
        FROM graphNodes \
        WHERE graphId = ? \
    ")
    query.addBindValue(graphId);
    EXEC(query)

    while (query.next()) {
        auto themeId = query.value(0).toInt();
        auto nodeId = query.value(1).toInt();
        auto* node = new GraphNode(nodeId);
        themeIdToNode[themeId] = node;
        addItem(node);
    }
    query.finish();

    // Add Edges
    PREPARE(query, " \
        WITH themeIds AS ( \
            SELECT themeId \
            FROM graphNodes \
            WHERE graphId = ? \
        ) \
        SELECT id, beginId, endId \
        FROM themeEdges \
        WHERE beginId IN themeIds \
          AND endId IN themeIds \
    ")
    query.addBindValue(graphId);
    EXEC(query)

    while (query.next()) {
        auto* edge = new GraphEdge(
            query.value(0).toInt(),
            themeIdToNode[query.value(1).toInt()],
            themeIdToNode[query.value(2).toInt()]
        );
        addItem(edge);
    }
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    auto pos = e->scenePos();
    int themeId;
    switch (mode) {
        case NEW_NODE_EDIT_MODE:
            if ((themeId = getThemeIdToAdd(pos)) != -1) {
                newNode(themeId, pos);
            }
            break;

        case EDGE_EDIT_MODE:
            pressedNode = typedItemAt<GraphNode*>(pos);
            edgePreviewLine = new QGraphicsLineItem(QLineF(pos, pos));
            addItem(edgePreviewLine);
            break;

        case CURSOR_EDIT_MODE:
            QGraphicsScene::mousePressEvent(e);
            break;

        case DELETE_EDIT_MODE:
            GraphNode* node;
            GraphEdge* edge;
            if (node = typedItemAt<GraphNode*>(pos)) {
                deleteNode(node);
            } else if (edge = typedItemAt<GraphEdge*>(pos)) {
                deleteEdge(edge);
            }
            break;
    }
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton) {
        if (mode == EDGE_EDIT_MODE && edgePreviewLine) {
            QLineF l(edgePreviewLine->line().p1(), e->scenePos());
            edgePreviewLine->setLine(l);
        }
    }

    // Set Status
    auto* item = itemAt(e->scenePos(), QTransform());
    GraphNode* node;
    GraphEdge* edge;
    if (node = qgraphicsitem_cast<GraphNode*>(item)) {
        emit showMessage(tr("[Node] %1").arg(node->toPlainText()));
    } else if (edge = qgraphicsitem_cast<GraphEdge*>(item)) {
        emit showMessage(tr("[Edge]"));
    } else {
        emit clearMessage();
    }

    QGraphicsScene::mouseMoveEvent(e);
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    switch (mode) {
        case EDGE_EDIT_MODE:
            removeItem(edgePreviewLine);
            delete edgePreviewLine;
            edgePreviewLine = nullptr;

            auto* releasedNode = typedItemAt<GraphNode*>(e->scenePos());

            if (!releasedNode || !pressedNode) {
                return;
            }

            newEdge(pressedNode, releasedNode);

            break;
    }

    QGraphicsScene::mouseReleaseEvent(e);
}

int GraphScene::getThemeIdToAdd(const QPointF& pos) const {
    ComboboxIdDialog d((QWidget*)views()[0]);
    d.setLabel(tr("Choose theme to add:"));
    d.addItem(tr("<New Node>"), -1);

    R_PREPARE_NEW(query, " \
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
    ", -1)
    query.addBindValue(graphId);
    R_EXEC(query, -1)

    while (query.next()) {
        d.addItem(
            QString("%1 (%2)")
                .arg(query.value(1).toString())
                .arg(query.value(2).toString()),
            query.value(0).toInt()
        );
    }

    if (d.exec() == QDialog::Rejected) {
        return -1;
    }

    auto themeId = d.getId();

    if (themeId == -1) {
        ThemeInfoDialog d(-1, (QWidget*)views()[0]);

        if (d.exec() == QDialog::Rejected) {
            return -1;
        }

        themeId = d.getId();
    }

    return themeId;
}

void GraphScene::newNode(int themeId, const QPointF& pos) {
    PREPARE_NEW(query, " \
        INSERT \
        INTO graphNodes(graphId, themeId, x, y) \
        VALUES (?, ?, ?, ?) \
    ")
    query.addBindValue(graphId);
    query.addBindValue(themeId);
    query.addBindValue(pos.x());
    query.addBindValue(pos.y());
    EXEC(query)

    auto* node = new GraphNode(query.lastInsertId().toInt());
    addItem(node);
    themeIdToNode[themeId] = node;

    query.finish();

    // Add edges
    PREPARE(query, " \
        WITH \
        themeIds AS ( \
            SELECT themeId \
            FROM graphNodes \
            WHERE graphId = :graphId \
        ) \
        SELECT id, beginId, endId \
        FROM themeEdges \
        WHERE ( \
                beginId = :themeId \
            AND endId in themeIds \
        ) OR ( \
                endId = :themeId \
            AND beginId in themeIds \
        ) \
    ")
    query.bindValue(":graphId", graphId);
    query.bindValue(":themeId", themeId);
    EXEC(query)

    while (query.next()) {
        auto* edge = new GraphEdge(
            query.value(0).toInt(),
            themeIdToNode[query.value(1).toInt()],
            themeIdToNode[query.value(2).toInt()]
        );
        addItem(edge);
    }

    emit graphsUpdated();
}

void GraphScene::newEdge(GraphNode* beginNode, GraphNode* endNode) {
    if (beginNode->isDeleted() || endNode->isDeleted()) {
        QMessageBox::critical(
            (QWidget*)views()[0],
            tr("Error"),
            tr("Cannot add edge to deleted node")
        );
        return;
    }

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

    if (!query.exec()) {
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
                return;
        }
    }

    auto* edge = new GraphEdge(
        query.lastInsertId().toInt(),
        beginNode,
        endNode
    );
    addItem(edge);
}

void GraphScene::deleteNode(GraphNode* node) {
    PREPARE_NEW(query, " \
        DELETE \
        FROM graphNodes \
        WHERE id = ? \
    ");
    query.addBindValue(node->getId());
    EXEC(query)

    removeItem(node);
    emit node->deleteEdges();
    node->deleteLater();
}

void GraphScene::deleteEdge(GraphEdge* edge) {
    PREPARE_NEW(query, " \
        DELETE \
        FROM themeEdges \
        WHERE id = ? \
    ");
    query.addBindValue(edge->getId());
    EXEC(query)

    removeItem(edge);
    edge->deleteLater();
}

void GraphScene::dragEnterEvent(QGraphicsSceneDragDropEvent* e) {
    if (e->mimeData()->hasFormat("application/x-themeid")) {
        e->acceptProposedAction();
    }
}

void GraphScene::dragMoveEvent(QGraphicsSceneDragDropEvent* e) {
    if (e->mimeData()->hasFormat("application/x-themeid")) {
        e->acceptProposedAction();
    }
}

void GraphScene::dropEvent(QGraphicsSceneDragDropEvent* e) {
    auto data = e->mimeData()->data("application/x-themeid");
    QDataStream stream(&data, QIODevice::ReadOnly);

    int themeId;
    stream >> themeId;

    if (themeIdToNode.contains(themeId)) {
        QMessageBox::critical(
            (QWidget*)views()[0],
            tr("Error"),
            tr("This node is already on the graph.")
        );
    } else {
        newNode(themeId, e->scenePos());
    }

    e->acceptProposedAction();
}
