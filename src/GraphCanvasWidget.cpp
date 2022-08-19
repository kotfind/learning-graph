#include "GraphCanvasWidget.h"

#include "GraphNodeWidget.h"

#include <QDebug>

GraphCanvasWidget::GraphCanvasWidget(QWidget* parent)
        : QFrame(parent) {
}

void GraphCanvasWidget::setMode(GraphEditMode mode) {
    this->mode = mode;
}

void GraphCanvasWidget::open(int graphId) {
    this->graphId = graphId;
    // TODO
}

void GraphCanvasWidget::mousePressEvent(QMouseEvent* e) {
    switch (mode) {
        case NEW_NODE_EDIT_MODE:
            auto* node = new GraphNodeWidget(this);
            node->move(e->pos());
            node->show();
            break;
    }
}
