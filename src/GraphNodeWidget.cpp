#include "GraphNodeWidget.h"

#include "GraphCanvasWidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

GraphNodeWidget::GraphNodeWidget(QWidget* parent)
        : QFrame(parent) {
    setFrameShape(QFrame::Box);

    auto* vbox = new QVBoxLayout;
    setLayout(vbox);

    auto* nameLabel = new QLabel("Node");
    vbox->addWidget(nameLabel);
}

void GraphNodeWidget::mousePressEvent(QMouseEvent* e) {
    dragPoint = e->pos();
}

void GraphNodeWidget::mouseMoveEvent(QMouseEvent* e) {
    if (((GraphCanvasWidget*)parent())->mode/* XXX */ == CURSOR_EDIT_MODE) {
        move(mapToParent(e->pos() - dragPoint));
    }
}

void GraphNodeWidget::mouseReleaseEvent(QMouseEvent* e) {
    qDebug() << "I'm on" << pos();
}
