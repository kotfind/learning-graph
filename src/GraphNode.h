#pragma once

// #include <QMouseEvent>
#include <QGraphicsSimpleTextItem>
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

    // protected:
    //     void mousePressEvent(QMouseEvent*) override;
    //     void mouseMoveEvent(QMouseEvent*) override;
    //     void mouseReleaseEvent(QMouseEvent*) override;
};
