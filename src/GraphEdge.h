#pragma once

#include "GraphNodeItem.h"

#include <QGraphicsObject>
#include <QRectF>
#include <QGraphicsScene>
#include <QPainterPath>

class GraphEdge : public QGraphicsObject {
    Q_OBJECT

    public:
        enum { Type = UserType + 1 };

        // Constructs edge with id edgeId
        // from beginNode to endNode
        // with parent parent
        GraphEdge(
            int edgeId,
            GraphNodeItem* beginNode,
            GraphNodeItem* endNode,
            QGraphicsItem* parent = nullptr
        );

        // Returns edge's id
        int getId() const { return edgeId; }

        // Returns edge's type
        int type() const override { return Type; }

        // Returns boundingRect
        QRectF boundingRect() const override;

        // Draws current edge on painter qp
        void paint(
            QPainter* qp,
            const QStyleOptionGraphicsItem*,
            QWidget* widget = nullptr) override;

        // Returns edge's shape
        QPainterPath shape() const override;

    private:
        // Returns edge's shape
        // if wideLine is true, then edge has width
        QPainterPath getPath(bool wideLine) const;

        int edgeId;

        const double arrowSize = 20;
        const double lineWidth = 6;

        GraphNodeItem* beginNode;
        GraphNodeItem* endNode;

        QPointF begin;
        QPointF end;

    public slots:
        // Updates edges position
        // Is called when one of nodes was moved
        void updatePosition();

        // Deletes itself
        void deleteSelf();
};
