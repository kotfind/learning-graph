#pragma once

#include <QPainter>
#include "GraphNode.h"

class GraphEdge : public QObject {
    Q_OBJECT

    public:
        GraphEdge(
            int edgeId,
            GraphNode* beginNode,
            GraphNode* endNode,
            QObject* parent = nullptr
        );

    private:
        int edgeId;
        GraphNode* beginNode;
        GraphNode* endNode;

    public slots:
        void draw(QPainter*);
};
