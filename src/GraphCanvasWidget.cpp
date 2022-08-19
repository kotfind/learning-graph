#include "GraphCanvasWidget.h"

GraphCanvasWidget::GraphCanvasWidget(QWidget* parent)
        : QFrame(parent) {
}

void GraphCanvasWidget::setMode(GraphEditMode mode) {
    this->mode = mode;
    qDebug() << mode;
}
