#include "GraphView.h"

#include "GraphNode.h"
#include "GraphEdge.h"

#include <QDebug>
#include <QPixmap>
#include <math.h>
#include <QApplication>
#include <QScrollBar>

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
    bool left = QApplication::mouseButtons() & Qt::LeftButton;
    bool middle = QApplication::mouseButtons() & Qt::MiddleButton;

    bool item = underCursorItem;
    bool node = qgraphicsitem_cast<GraphNode*>(underCursorItem);
    bool edge = qgraphicsitem_cast<GraphEdge*>(underCursorItem);

    if (middle) {
        cursor = QCursor(Qt::ClosedHandCursor);
    } else {
        cursor = QCursor(Qt::OpenHandCursor);
    }

    switch (mode) {
        case NEW_NODE_EDIT_MODE:
            cursor = QCursor(QPixmap(":plus.svg"));
            break;

        case EDGE_EDIT_MODE:
                if (node) {
                    cursor = QCursor(QPixmap(":arrow.svg"), 0, -1);
                } else if (left) {
                    cursor = QCursor(Qt::ForbiddenCursor);
                }
            break;

        case CURSOR_EDIT_MODE:
            if (node) {
                cursor = QCursor(Qt::ArrowCursor);
            }
            break;

        case DELETE_EDIT_MODE:
            if (item) {
                cursor = QCursor(QPixmap(":cross.svg"));
            }
            break;
    }

    viewport()->setCursor(cursor);
}

void GraphView::mousePressEvent(QMouseEvent* e) {
    QGraphicsView::mousePressEvent(e);
    updateCursor();

    lastMovePoint = e->pos();
}

void GraphView::mouseMoveEvent(QMouseEvent* e) {
    QGraphicsView::mouseMoveEvent(e);
    updateCursor();

    if (e->buttons() & Qt::MiddleButton) {
        auto delta = e->pos() - lastMovePoint;
        lastMovePoint = e->pos();

        auto* vbar = verticalScrollBar();
        auto* hbar = horizontalScrollBar();
        hbar->setValue(hbar->value() - delta.x());
        vbar->setValue(vbar->value() - delta.y());
    }
}

void GraphView::mouseReleaseEvent(QMouseEvent* e) {
    QGraphicsView::mouseReleaseEvent(e);
    updateCursor();
}
