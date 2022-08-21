#pragma once

#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QRectF>

class GraphNode : public QGraphicsTextItem {
    public:
        enum { Type = UserType + 0 };

        GraphNode(int nodeId, QGraphicsItem* parent = nullptr);

        int type() const override { return Type; }

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
