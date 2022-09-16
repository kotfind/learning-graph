#pragma once

#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QRectF>

class GraphNodeItem : public QGraphicsTextItem {
    Q_OBJECT

    public:
        enum { Type = UserType + 0 };

        GraphNodeItem(int nodeId, QGraphicsItem* parent = nullptr);

        int type() const override { return Type; }

        void paint(
            QPainter*,
            const QStyleOptionGraphicsItem*,
            QWidget *widget = nullptr
        );

        int getId() const { return nodeId; }

        bool intersect(const QLineF&, QPointF*);

        bool isDeleted() { return deleted; }

    private:
        int nodeId;
        bool deleted; // if node's theme was deleted

    protected:
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;

    signals:
        void positionChanged();
        void deleteEdges();

    private slots:
        void load();
};
