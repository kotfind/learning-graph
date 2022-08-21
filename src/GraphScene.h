#pragma once

#include "GraphEditMode.h"
#include "GraphNode.h"

#include <QGraphicsScene>
// #include <QMouseEvent>
#include <QPoint>

class GraphScene : public QGraphicsScene {
    Q_OBJECT

    public:
        GraphScene();

    private:
        void newNode(QPoint pos);

        int graphId;

        GraphEditMode mode;

    // protected:
    //     void mousePressEvent(QMouseEvent*) override;

    signals:
        void graphsUpdated();

    public slots:
        void open(int graphId);

        void setMode(GraphEditMode mode);

    friend GraphNode;
};
