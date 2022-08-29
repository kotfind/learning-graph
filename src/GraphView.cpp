#include "GraphView.h"

#include <QDebug>
#include <QPixmap>

GraphView::GraphView(QWidget* parent)
        : QGraphicsView(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    // setDragMode(QGraphicsView::ScrollHandDrag);
}

void GraphView::setMode(GraphEditMode mode) {
    switch (mode) {
        case NEW_NODE_EDIT_MODE:
            setCursor(QCursor(QPixmap(":plus.svg")));
            break;

        case EDGE_EDIT_MODE:
            setCursor(QCursor(QPixmap(":arrow.svg"), 0, -1));
            break;

        case CURSOR_EDIT_MODE:
            setCursor(QCursor(QPixmap(":pointer.svg"), 0, 0));
            break;

        case DELETE_EDIT_MODE:
            setCursor(QCursor(QPixmap(":cross.svg")));
            break;
    }
}
