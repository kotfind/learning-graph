#pragma once

#include "GraphEditMode.h"

#include <QGraphicsView>
#include <QWheelEvent>

class GraphView : public QGraphicsView {
    Q_OBJECT

    public:
        GraphView(QWidget* parent = nullptr);

    protected:
        void wheelEvent(QWheelEvent*) override;

    private:
        const double scaleDelthaFactor = 5e-4;

    signals:
        void scaleChanged(double);

    public slots:
        void setMode(GraphEditMode);
        void setScale(double);
};
