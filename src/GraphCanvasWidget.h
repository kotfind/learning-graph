#pragma once

#include <QFrame>

#include "GraphEditMode.h"

class GraphCanvasWidget : public QFrame {
    Q_OBJECT

    public:
        GraphCanvasWidget(QWidget* parent = nullptr);

    private:
        int graphId;

        GraphEditMode mode;

    public slots:
        void open(int graphId);

        void setMode(GraphEditMode mode);
};
