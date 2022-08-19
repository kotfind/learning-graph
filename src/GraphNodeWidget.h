#pragma once

#include <QFrame>
#include <QMouseEvent>
#include <QPoint>

class GraphNodeWidget : public QFrame {
    public:
        GraphNodeWidget(QWidget* parent = nullptr);

    private:
        int nodeId;

        QPoint dragPoint;

    protected:
        void mousePressEvent(QMouseEvent*) override;
        void mouseMoveEvent(QMouseEvent*) override;
        void mouseReleaseEvent(QMouseEvent*) override;
};
