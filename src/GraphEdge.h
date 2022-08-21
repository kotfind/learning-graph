#pragma once

#include "GraphNode.h"

#include <QGraphicsObject>
#include <QRectF>

class GraphEdge : public QGraphicsObject {
    Q_OBJECT

    public:
        enum { Type = UserType + 1 };

        GraphEdge(
            int edgeId,
            GraphNode* beginNode,
            GraphNode* endNode,
            QGraphicsItem* parent = nullptr
        );

        int type() const override { return Type; }

        QRectF boundingRect() const override;

        void paint(
            QPainter*,
            const QStyleOptionGraphicsItem*,
            QWidget* widget = nullptr) override;

    private:
        int edgeId;

        GraphNode* beginNode;
        GraphNode* endNode;

        QPointF begin;
        QPointF end;

    public slots:
        void updatePosition();
};
