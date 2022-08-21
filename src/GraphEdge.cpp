#include "GraphEdge.h"

#include "sqlDefines.h"
#include <QDebug>

GraphEdge::GraphEdge(
    int edgeId,
    GraphNode* beginNode,
    GraphNode* endNode,
    QGraphicsItem* parent)
        : QGraphicsObject(parent),
          edgeId(edgeId),
          beginNode(beginNode),
          endNode(endNode) {

    connect(
        beginNode,
        &GraphNode::positionChanged,
        this,
        &GraphEdge::updatePosition
    );

    connect(
        endNode,
        &GraphNode::positionChanged,
        this,
        &GraphEdge::updatePosition
    );

    updatePosition();
}

QRectF GraphEdge::boundingRect() const {
    auto x1 = begin.x();
    auto y1 = begin.y();
    auto x2 = end.x();
    auto y2 = end.y();

    return QRectF(
        std::min(x1, x2),
        std::min(y1, y2),
        std::abs(x1 - x2),
        std::abs(y1 - y2)
    );
}

void GraphEdge::updatePosition() {
    prepareGeometryChange();
    begin = beginNode->pos();
    end = endNode->pos();
}

void GraphEdge::paint(
    QPainter* qp,
    const QStyleOptionGraphicsItem*,
    QWidget*) {

    // FIXME: fit in boundingRect
    qp->drawLine(beginNode->pos(), endNode->pos());
}
