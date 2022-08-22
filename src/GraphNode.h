#pragma once

#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QRectF>

class GraphNode : public QGraphicsTextItem {
    Q_OBJECT

    public:
        enum { Type = UserType + 0 };

        GraphNode(int nodeId, QGraphicsItem* parent = nullptr);

        int type() const override { return Type; }

        void paint(
            QPainter*,
            const QStyleOptionGraphicsItem*,
            QWidget *widget = nullptr
        );

        int getId() const { return nodeId; }

        bool intersect(const QLineF&, QPointF*);

    private:
        int nodeId;

    protected:
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;

    signals:
        void positionChanged();

    private slots:
        void load();
};
