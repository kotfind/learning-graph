#pragma once

#include <QGraphicsView>
#include "GraphEditMode.h"

class GraphView : public QGraphicsView {
    public:
        GraphView(QWidget* parent = nullptr);

    public slots:
        void setMode(GraphEditMode);
};
