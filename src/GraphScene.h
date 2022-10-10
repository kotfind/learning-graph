#pragma once

#include "GraphEditMode.h"
#include "GraphNodeItem.h"
#include "GraphEdge.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QGraphicsLineItem>
#include <QHash>
#include <QGraphicsSceneDragDropEvent>

class GraphScene : public QGraphicsScene {
    Q_OBJECT

    public:
        GraphScene();

        GraphEditMode getMode() { return mode; }

    private:
        template<typename T> T typedItemAt(const QPointF& pos);

        int getThemeIdToAdd(const QPointF& pos) const;

        void newNode(int themeId, const QPointF& pos);
        void newEdge(GraphNodeItem* beginNode, GraphNodeItem* endNode);

        void deleteNode(GraphNodeItem*);
        void deleteEdge(GraphEdge*);

        int graphId;

        GraphEditMode mode = CURSOR_EDIT_MODE;

        GraphNodeItem* pressedNode = nullptr;
        QGraphicsLineItem* edgePreviewLine = nullptr;

        QHash<int, GraphNodeItem*> themeIdToNode;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;

        void dragEnterEvent(QGraphicsSceneDragDropEvent*) override;
        void dragMoveEvent(QGraphicsSceneDragDropEvent*) override;
        void dropEvent(QGraphicsSceneDragDropEvent*) override;

    signals:
        void graphsUpdated();

    public slots:
        void open(int graphId);
        void setMode(GraphEditMode mode);
        void recalcSceneRect();
};
