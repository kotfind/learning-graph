#pragma once

#include "GraphEditMode.h"

#include <QFrame>
#include <QMouseEvent>

class GraphCanvasWidget : public QFrame {
    Q_OBJECT

    public:
        GraphCanvasWidget(QWidget* parent = nullptr);

    private:
        int graphId;

        GraphEditMode mode;

    protected:
        void mousePressEvent(QMouseEvent*) override;

    public slots:
        void open(int graphId);

        void setMode(GraphEditMode mode);
};
