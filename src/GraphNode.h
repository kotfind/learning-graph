#pragma once

#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QRectF>

class GraphNode : public QGraphicsTextItem {
    public:
        GraphNode(int nodeId, QGraphicsItem* parent = nullptr);

        void paint(
            QPainter*,
            const QStyleOptionGraphicsItem*,
            QWidget *widget = nullptr
        );

    private:
        int nodeId;

    private slots:
        void load();

    protected:
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
};
