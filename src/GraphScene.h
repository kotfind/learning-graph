#pragma once

#include "GraphEditMode.h"
#include "GraphNode.h"
#include "GraphEdge.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QGraphicsLineItem>

class GraphScene : public QGraphicsScene {
    Q_OBJECT

    public:
        GraphScene();

    private:
        template<typename T> T typedItemAt(const QPointF& pos);

        void newNode(const QPointF& pos);
        void newEdge(GraphNode* beginNode, GraphNode* endNode);

        void deleteNode(GraphNode*);
        void deleteEdge(GraphEdge*);

        int graphId;

        GraphEditMode mode = CURSOR_EDIT_MODE;

        GraphNode* pressedNode = nullptr;
        QGraphicsLineItem* edgePreviewLine = nullptr;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;

    signals:
        void graphsUpdated();

    public slots:
        void open(int graphId);

        void setMode(GraphEditMode mode);
};
