#include "GraphView.h"

GraphView::GraphView(QWidget* parent)
        : QGraphicsView(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    // setDragMode(QGraphicsView::ScrollHandDrag);
}
