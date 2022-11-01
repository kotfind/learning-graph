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
        // Scales graph
        void wheelEvent(QWheelEvent*) override;

        // Inits updateCursor
        // Sets moveStartPoint
        void mousePressEvent(QMouseEvent*) override;

        // Inits updateCursor
        // Moves graph
        void mouseMoveEvent(QMouseEvent*) override;

        // Inits updateCursor
        void mouseReleaseEvent(QMouseEvent*) override;

    private:
        // Updates cursor
        // Is called by mouse event handlers
        void updateCursor(QMouseEvent*);

        const double scaleDelthaFactor = 5e-4;

        QCursor cursor;

        QPoint moveStartPoint;

    signals:
        void scaleChanged(double);

    public slots:
        // Sets scale to s
        void setScale(double s);
};
