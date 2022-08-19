#pragma once

#include <QFrame>

#include "GraphEditMode.h"

class GraphCanvasWidget : public QFrame {
    Q_OBJECT

    public:
        GraphCanvasWidget(QWidget* parent = nullptr);

    private:
        GraphEditMode mode;

    public slots:
        void setMode(GraphEditMode mode);
};
