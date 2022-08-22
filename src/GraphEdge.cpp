#include "GraphEdge.h"

#include "sqlDefines.h"
#include <QDebug>
#include <QPainterPath>
#include <QPolygonF>

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
        std::min(x1, x2)    - arrowSize,
        std::min(y1, y2)    - arrowSize,
        std::abs(x1 - x2)   + arrowSize * 2,
        std::abs(y1 - y2)   + arrowSize * 2
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

    // Set Brush
    qp->setBrush(Qt::black);

    // Draw Arrow
    QLineF l(begin, end);
    qp->drawLine(l);

    auto unit = l.unitVector();
    auto norm = unit.normalVector();

    QPointF front(unit.dx(), unit.dy());
    QPointF right(norm.dx(), norm.dy());

    auto base = l.p1() + front * (l.length() - arrowSize);
    auto deltha = right * arrowSize / 2;

    QPolygonF arrow;
    arrow << l.p2()
        << base + deltha
        << base + front * arrowSize / 3
        << base - deltha;

    qp->drawPolygon(arrow);
}
