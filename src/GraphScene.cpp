#include "GraphScene.h"

#include "ComboboxIdDialog.h"
#include "db/db.h"
#include "GlobalSignalHandler.h"
#include "ThemeInfoDialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QHash>
#include <QMimeData>
#include <QRectF>
#include <QMarginsF>
#include <QGraphicsView>
#include <qnamespace.h>

GraphScene::GraphScene()
        : QGraphicsScene() {
    connect(
        this,
        &GraphScene::graphsUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::graphsUpdated
    );

    connect(
        this,
        &GraphScene::changed,
        this,
        &GraphScene::recalcSceneRect
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

    auto nodes = db::graphNode::reads(graphId);
    for (const auto& n : nodes) {
        auto* node = new GraphNodeItem(n.id);
        themeIdToNode[n.themeId] = node;
        addItem(node);
    }

    auto edges = db::themeEdge::reads(graphId, -1);
    for (const auto& e : edges) {
        auto* edge = new GraphEdge(
            e.id,
            themeIdToNode[e.beginId],
            themeIdToNode[e.endId]
        );
        addItem(edge);
    }
}

void GraphScene::close() {
    this->graphId = -1;

    clear();
    themeIdToNode.clear();
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    if (!(e->buttons() & Qt::LeftButton)) {
        return;
    }

    auto pos = e->scenePos();
    int themeId;
    switch (mode) {
        case NEW_NODE_EDIT_MODE:
            if ((themeId = getThemeIdToAdd()) != -1) {
                newNode(themeId, pos);
            }
            break;

        case EDGE_EDIT_MODE:
            pressedNode = typedItemAt<GraphNodeItem*>(pos);
            edgePreviewLine = new QGraphicsLineItem(QLineF(pos, pos));
            addItem(edgePreviewLine);
            break;

        case CURSOR_EDIT_MODE:
            QGraphicsScene::mousePressEvent(e);
            break;

        case DELETE_EDIT_MODE:
            GraphNodeItem* node;
            GraphEdge* edge;
            if (node = typedItemAt<GraphNodeItem*>(pos)) {
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

    QGraphicsScene::mouseMoveEvent(e);
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    switch (mode) {
        case EDGE_EDIT_MODE:
            removeItem(edgePreviewLine);
            delete edgePreviewLine;
            edgePreviewLine = nullptr;

            auto* releasedNode = typedItemAt<GraphNodeItem*>(e->scenePos());

            if (!releasedNode || !pressedNode) {
                return;
            }

            newEdge(pressedNode, releasedNode);

            break;
    }

    QGraphicsScene::mouseReleaseEvent(e);
}

int GraphScene::getThemeIdToAdd() const {
    ComboboxIdDialog d((QWidget*)views()[0]);
    d.setLabel(tr("Choose theme to add:"));
    d.addItem(tr("<New Theme>"), -1);

    auto themes = db::theme::readsExceptGraph(graphId);

    for (const auto t : themes) {
        d.addItem(
            QString("%1 (%2)")
                .arg(t.name)
                .arg(t.package.name),
            t.id
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
    GraphNode n;
    n.graphId = graphId;
    n.themeId = themeId;
    n.x = pos.x();
    n.y = pos.y();
    auto nodeId = db::graphNode::create(n);

    auto* node = new GraphNodeItem(nodeId);
    addItem(node);
    themeIdToNode[themeId] = node;

    // Add edges
    auto edges = db::themeEdge::reads(graphId, themeId);
    for (const auto& e : edges) {
        auto* edge = new GraphEdge(
            e.id,
            themeIdToNode[e.beginId],
            themeIdToNode[e.endId]
        );
        addItem(edge);
    }

    emit graphsUpdated();
}

void GraphScene::newEdge(GraphNodeItem* beginNode, GraphNodeItem* endNode) {
    if (beginNode->isDeleted() || endNode->isDeleted()) {
        QMessageBox::critical(
            (QWidget*)views()[0],
            tr("Error"),
            tr("Cannot add edge to deleted node")
        );
        return;
    }

    try {
        int edgeId = db::themeEdge::create(beginNode->getId(), endNode->getId());

        auto* edge = new GraphEdge(
            edgeId,
            beginNode,
            endNode
        );
        addItem(edge);
    } catch (const QString& msg) {
        QMessageBox::critical(
            (QWidget*)views()[0],
            tr("Error"),
            msg
        );
    } catch (...) {
        return;
    }
}

void GraphScene::deleteNode(GraphNodeItem* node) {
    db::graphNode::del(node->getId());
    removeItem(node);
    emit node->deleteEdges();
    node->deleteLater();
}

void GraphScene::deleteEdge(GraphEdge* edge) {
    db::themeEdge::del(edge->getId());
    removeItem(edge);
    edge->deleteLater();
}

void GraphScene::dragEnterEvent(QGraphicsSceneDragDropEvent* e) {
    if (e->mimeData()->hasFormat("application/x-themeid")) {
        e->acceptProposedAction();
    } else {
        e->ignore();
    }
}

void GraphScene::dragMoveEvent(QGraphicsSceneDragDropEvent* e) {
    if (e->mimeData()->hasFormat("application/x-themeid")) {
        e->acceptProposedAction();
    } else {
        e->ignore();
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

void GraphScene::recalcSceneRect() {
    auto r = itemsBoundingRect();
    const auto& w = r.width();
    const auto& h = r.height();
    r += QMargins(w, h, w, h);
    setSceneRect(r.united(views()[0]->sceneRect()));
}
