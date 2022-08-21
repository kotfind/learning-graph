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

    qDebug() << "NEW EDGE";
    qDebug() << beginNode;
    qDebug() << endNode;
    qDebug() << "";
    // TODO
}

QRectF GraphEdge::boundingRect() const {
    return QRectF(0, 0, 0, 0); // TODO
}

void GraphEdge::updatePosition() {
    // TODO
}

void GraphEdge::paint(
    QPainter*,
    const QStyleOptionGraphicsItem*,
    QWidget*) {

    // TODO
}
