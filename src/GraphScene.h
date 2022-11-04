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

        // Returns graph's edit mode
        GraphEditMode getMode() { return mode; }

        // Returns item with current type in position pos
        //     or nullptr otherwise
        template<typename T> T typedItemAt(const QPointF& pos);

        // Returns current graphId
        int getGraphId() const { return graphId; }

    private:
        // Asks user to select a theme to add
        // Returns id if user selected theme
        // Returns -1 otherwise
        // NOTE: user can create new theme in progress
        int getThemeIdToAdd() const;

        // Creates new node from theme with id themeId on position pos
        void newNode(int themeId, const QPointF& pos);

        // Creates new edge from beginNode to endNode
        void newEdge(GraphNodeItem* beginNode, GraphNodeItem* endNode);

        // Deletes node node
        void deleteNode(GraphNodeItem* node);

        // Deletes edge edge
        void deleteEdge(GraphEdge* edge);

        int graphId = -1;

        GraphEditMode mode = CURSOR_EDIT_MODE;

        GraphNodeItem* pressedNode = nullptr;
        QGraphicsLineItem* edgePreviewLine = nullptr;

        QHash<int, GraphNodeItem*> themeIdToNode;

    protected:
        // Runs one of other class methods
        //    depending on mode and mouse button
        // Inits ThemeContextMenu on right click
        void mousePressEvent(QGraphicsSceneMouseEvent *) override;

        // Draws preview line in EDGE_EDIT_MODE
        void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;

        // Inits edge creation in EDGE_EDIT_MODE
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;

        // Filters drag&drop events
        void dragEnterEvent(QGraphicsSceneDragDropEvent *) override;

        // Filters drag&drop events
        void dragMoveEvent(QGraphicsSceneDragDropEvent *) override;

        // Creates node from dropped theme
        void dropEvent(QGraphicsSceneDragDropEvent *) override;

    signals:
        void graphsUpdated();

    public slots:
        // Opens graph with id graphId
        void open(int graphId);

        // Closes current graph
        void close();

        // Sets mode to mode
        void setMode(GraphEditMode mode);

        // Recalculates scene rect
        void recalcSceneRect();
};
