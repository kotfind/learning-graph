#pragma once

#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QRectF>

class GraphNode : public QGraphicsSimpleTextItem {
    public:
        GraphNode(int nodeId, QGraphicsItem* parent = nullptr);

        QRectF boundingRect() const override;

        void paint(
            QPainter*,
            const QStyleOptionGraphicsItem*,
            QWidget *widget = nullptr
        );

    private:
        int nodeId;

        const double margin = 10;

    private slots:
        void load();

    protected:
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
};
