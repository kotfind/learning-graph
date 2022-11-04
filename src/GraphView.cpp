#include "GraphView.h"

#include "GraphNodeItem.h"
#include "GraphEdge.h"
#include "GraphScene.h"

#include <QDebug>
#include <QPixmap>
#include <math.h>
#include <QApplication>
#include <QScrollBar>

#define SCENE ((GraphScene*)scene())

GraphView::GraphView(QWidget* parent)
        : QGraphicsView(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    // setDragMode(QGraphicsView::ScrollHandDrag);
    setMouseTracking(true);
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

void GraphView::updateCursor(QMouseEvent* e) {
    bool left = e->buttons() & Qt::LeftButton;
    bool middle = e->buttons() & Qt::MiddleButton;

    auto pos = mapToScene(e->pos());
    bool item = SCENE->itemAt(pos, QTransform());
    bool node = SCENE->typedItemAt<GraphNodeItem*>(pos);
    bool edge = SCENE->typedItemAt<GraphEdge*>(pos);

    if (middle) {
        cursor = QCursor(Qt::ClosedHandCursor);
    } else {
        cursor = QCursor(Qt::OpenHandCursor);
    }

    switch (SCENE->getMode()) {
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
    updateCursor(e);

    moveStartPoint = e->pos();

    e->ignore(); // pass to parent
}

void GraphView::mouseMoveEvent(QMouseEvent* e) {
    QGraphicsView::mouseMoveEvent(e);
    updateCursor(e);

    if (e->buttons() & Qt::MiddleButton) {
        auto delta = e->pos() - moveStartPoint;
        moveStartPoint = e->pos();

        auto* vbar = verticalScrollBar();
        auto* hbar = horizontalScrollBar();
        hbar->setValue(hbar->value() - delta.x());
        vbar->setValue(vbar->value() - delta.y());
    }

    e->ignore(); // pass to parent
}

void GraphView::mouseReleaseEvent(QMouseEvent* e) {
    QGraphicsView::mouseReleaseEvent(e);
    updateCursor(e);

    e->ignore(); // pass to parent
}
