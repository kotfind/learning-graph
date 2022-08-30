#include "GraphView.h"

#include "GraphNode.h"
#include "GraphEdge.h"

#include <QDebug>
#include <QPixmap>
#include <math.h>
#include <QApplication>

GraphView::GraphView(QWidget* parent)
        : QGraphicsView(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    // setDragMode(QGraphicsView::ScrollHandDrag);
    setMouseTracking(true);
}

void GraphView::setMode(GraphEditMode m) {
    mode = m;
}

void GraphView::setUnderCursorItem(QGraphicsItem* item) {
    underCursorItem = item;
}

void GraphView::wheelEvent(QWheelEvent* e) {
    if (e->modifiers() & Qt::ControlModifier) {
        auto s = pow(2, e->angleDelta().y() * scaleDelthaFactor);
        scale(s, s);
        emit scaleChanged(transform().m11());
    } else {
        e->ignore();
        QGraphicsView::wheelEvent(e);
    }
}

void GraphView::setScale(double s) {
    s /= transform().m11();
    scale(s, s);
}

void GraphView::updateCursor() {
    if (QApplication::mouseButtons() & Qt::LeftButton) {
        cursor = QCursor(Qt::ClosedHandCursor);
    } else {
        cursor = QCursor(Qt::OpenHandCursor);
    }

    switch (mode) {
        case NEW_NODE_EDIT_MODE:
            cursor = QCursor(QPixmap(":plus.svg"));
            break;

        case EDGE_EDIT_MODE:
            if (qgraphicsitem_cast<GraphNode*>(underCursorItem)) {
                cursor = QCursor(QPixmap(":arrow.svg"), 0, -1);
            }
            break;

        case CURSOR_EDIT_MODE:
            if (qgraphicsitem_cast<GraphNode*>(underCursorItem)) {
                cursor = QCursor(Qt::ArrowCursor);
            }
            break;

        case DELETE_EDIT_MODE:
            if (underCursorItem) {
                cursor = QCursor(QPixmap(":cross.svg"));
            }
            break;
    }

    viewport()->setCursor(cursor);
}

void GraphView::mousePressEvent(QMouseEvent* e) {
    QGraphicsView::mousePressEvent(e);
    updateCursor();
}

void GraphView::mouseMoveEvent(QMouseEvent* e) {
    QGraphicsView::mouseMoveEvent(e);
    updateCursor();
}
void GraphView::mouseReleaseEvent(QMouseEvent* e) {
    QGraphicsView::mouseReleaseEvent(e);
    updateCursor();
}
