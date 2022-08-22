#pragma once

#include "GraphEditMode.h"
#include "GraphNode.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>

class GraphScene : public QGraphicsScene {
    Q_OBJECT

    public:
        GraphScene();

    private:
        template<typename T> T typedItemAt(const QPointF& pos);

        void newNode(const QPointF& pos);
        void newEdge(GraphNode* beginNode, GraphNode* endNode);

        int graphId;

        GraphEditMode mode = CURSOR_EDIT_MODE;

        GraphNode* pressedNode = nullptr;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;

    signals:
        void graphsUpdated();

    public slots:
        void open(int graphId);

        void setMode(GraphEditMode mode);
};
