#include "GraphEdge.h"

#include "sqlDefines.h"
#include <QDebug>

GraphEdge::GraphEdge(
    int edgeId,
    GraphNode* beginNode,
    GraphNode* endNode,
    QObject* parent)
        : QObject(parent),
          edgeId(edgeId),
          beginNode(beginNode),
          endNode(endNode) {

    qDebug() << "NEW EDGE" << beginNode << endNode;
}

void GraphEdge::draw(QPainter*) {

}
