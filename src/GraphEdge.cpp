#include "GraphEdge.h"

#include <QDebug>
#include <QPainterPath>
#include <QPolygonF>

GraphEdge::GraphEdge(
    int edgeId,
    GraphNodeItem* beginNode,
    GraphNodeItem* endNode,
    QGraphicsItem* parent)
        : QGraphicsObject(parent),
          edgeId(edgeId),
          beginNode(beginNode),
          endNode(endNode) {

    connect(
        beginNode,
        &GraphNodeItem::positionChanged,
        this,
        &GraphEdge::updatePosition
    );

    connect(
        endNode,
        &GraphNodeItem::positionChanged,
        this,
        &GraphEdge::updatePosition
    );

    connect(
        beginNode,
        &GraphNodeItem::deleteEdges,
        this,
        &GraphEdge::deleteSelf
    );

    connect(
        endNode,
        &GraphNodeItem::deleteEdges,
        this,
        &GraphEdge::deleteSelf
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

    QLineF l(
        mapFromItem(beginNode, beginNode->boundingRect().center()),
        mapFromItem(endNode, endNode->boundingRect().center())
    );

    beginNode->intersect(l, &begin); // XXX if returns false
    endNode->intersect(l, &end); // XXX if returns false
}

QPainterPath GraphEdge::getPath(bool wideLine) const {
    if (beginNode->collidesWithItem(endNode)) {
        return QPainterPath();
    }

    QLineF l(begin, end);

    auto unit = l.unitVector();
    auto norm = unit.normalVector();

    QPointF front(unit.dx(), unit.dy());
    QPointF right(norm.dx(), norm.dy());

    QPainterPath pp;

    // Line
    if (wideLine) {
        auto deltha = right * lineWidth / 2;
        pp.moveTo(begin + deltha);
        pp.lineTo(begin - deltha);
        pp.lineTo(end   - deltha);
        pp.lineTo(end   + deltha);
    } else {
        pp.moveTo(begin);
        pp.lineTo(end);
    }
    pp.closeSubpath();

    // Arrow
    auto base = l.p1() + front * (l.length() - arrowSize);
    auto deltha = right * arrowSize / 2;

    pp.moveTo(end);
    pp.lineTo(base + deltha);
    pp.lineTo(base + front * arrowSize / 3);
    pp.lineTo(base - deltha);
    pp.lineTo(end);

    return pp;
}

QPainterPath GraphEdge::shape() const {
    return getPath(true);
}

void GraphEdge::paint(
    QPainter* qp,
    const QStyleOptionGraphicsItem*,
    QWidget*) {

    qp->setBrush(Qt::black);
    qp->drawPath(getPath(false));
}

void GraphEdge::deleteSelf() {
    // May be false if both ends were deleted simultaneously
    // (when deleting package)
    if (scene()) {
        scene()->removeItem(this);
        deleteLater();
    }
}
