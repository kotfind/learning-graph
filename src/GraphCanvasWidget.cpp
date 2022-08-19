#include "GraphCanvasWidget.h"

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
