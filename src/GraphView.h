#pragma once

#include "GraphEditMode.h"

#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QGraphicsItem>

class GraphView : public QGraphicsView {
    Q_OBJECT

    public:
        GraphView(QWidget* parent = nullptr);

    protected:
        void wheelEvent(QWheelEvent*) override;
        void mousePressEvent(QMouseEvent*) override;
        void mouseMoveEvent(QMouseEvent*) override;
        void mouseReleaseEvent(QMouseEvent*) override;

    private:
        void updateCursor();

        const double scaleDelthaFactor = 5e-4;

        QCursor cursor = QCursor();
        GraphEditMode mode = CURSOR_EDIT_MODE;
        QGraphicsItem* underCursorItem = nullptr;

    signals:
        void scaleChanged(double);

    public slots:
        void setScale(double);
        void setunderCursorItem(QGraphicsItem*);
        void setMode(GraphEditMode);
};
