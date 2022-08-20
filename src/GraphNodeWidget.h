#pragma once

#include <QFrame>
#include <QMouseEvent>
#include <QPoint>
#include <QLabel>

class GraphNodeWidget : public QFrame {
    public:
        GraphNodeWidget(int nodeId, QWidget* parent = nullptr);

    private:
        void ui();
        void load();

        int nodeId;

        QPoint dragPoint;
        QLabel* nameLabel;

    protected:
        void mousePressEvent(QMouseEvent*) override;
        void mouseMoveEvent(QMouseEvent*) override;
        void mouseReleaseEvent(QMouseEvent*) override;
};
