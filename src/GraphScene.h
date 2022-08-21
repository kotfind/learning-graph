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
        void newNode(QPointF pos);

        int graphId;

        GraphEditMode mode;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*) override;

    signals:
        void graphsUpdated();

    public slots:
        void open(int graphId);

        void setMode(GraphEditMode mode);

    friend GraphNode;
};