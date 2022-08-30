#pragma once

#include "GraphEditMode.h"

#include <QGraphicsView>
#include <QWheelEvent>

class GraphView : public QGraphicsView {
    public:
        GraphView(QWidget* parent = nullptr);

    protected:
        void wheelEvent(QWheelEvent*) override;

    private:
        const double scaleDelthaFactor = 5e-4;

    public slots:
        void setMode(GraphEditMode);
};
