#include "GraphNodeWidget.h"

#include "GraphCanvasWidget.h"
#include "sqlDefines.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

GraphNodeWidget::GraphNodeWidget(int nodeId, QWidget* parent)
        : QFrame(parent), nodeId(nodeId) {
    setFrameShape(QFrame::Box);

    auto* vbox = new QVBoxLayout;
    setLayout(vbox);

    auto* nameLabel = new QLabel(QString("id: %1").arg(nodeId)); // FIXME
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
