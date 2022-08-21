#pragma once

#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

class GraphNode : public QGraphicsSimpleTextItem {
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
