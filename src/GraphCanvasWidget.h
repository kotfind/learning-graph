#pragma once

#include "GraphEditMode.h"
#include "GraphNodeWidget.h"

#include <QFrame>
#include <QMouseEvent>
#include <QPoint>

class GraphCanvasWidget : public QFrame {
    Q_OBJECT

    public:
        GraphCanvasWidget(QWidget* parent = nullptr);

    private:
        void newNode(QPoint pos);

        int graphId;

        GraphEditMode mode;

    protected:
        void mousePressEvent(QMouseEvent*) override;

    public slots:
        void open(int graphId);

        void setMode(GraphEditMode mode);

    friend GraphNodeWidget;
};
