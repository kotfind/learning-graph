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

        GraphEdge(
            int edgeId,
            GraphNodeItem* beginNode,
            GraphNodeItem* endNode,
            QGraphicsItem* parent = nullptr
        );

        int getId() const { return edgeId; }

        int type() const override { return Type; }

        QRectF boundingRect() const override;

        void paint(
            QPainter*,
            const QStyleOptionGraphicsItem*,
            QWidget* widget = nullptr) override;

        QPainterPath shape() const override;

    private:
        QPainterPath getPath(bool wideLine) const;

        int edgeId;

        const double arrowSize = 20;
        const double lineWidth = 6;

        GraphNodeItem* beginNode;
        GraphNodeItem* endNode;

        QPointF begin;
        QPointF end;

    public slots:
        void updatePosition();
        void deleteSelf();
};
