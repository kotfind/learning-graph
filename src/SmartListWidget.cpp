#include "SmartListWidget.h"

#include <QListWidgetItem>
#include <QApplication>

SmartListWidget::SmartListWidget(QWidget* parent) 
        : QListWidget(parent) {
    setContextMenuPolicy(Qt::DefaultContextMenu);
}

void SmartListWidget::addItem(const QString& name, int id) {
    auto* item = new QListWidgetItem(name);
    item->setData(Qt::UserRole, id);
    QListWidget::addItem(item);
}

int SmartListWidget::currentId() {
    return currentItem()->data(Qt::UserRole).toInt();
}

void SmartListWidget::mouseDoubleClickEvent(QMouseEvent* e) {
    if (currentItem()) {
        emit doubleClicked(currentId());
    }
}

void SmartListWidget::contextMenuEvent(QContextMenuEvent* e) {
    if (currentItem()) {
        emit menuRequested(currentId(), e->globalPos());
    }
}

void SmartListWidget::mousePressEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton) {
        dragStartPoint = e->pos();
    }

    QListWidget::mousePressEvent(e);
}

void SmartListWidget::mouseMoveEvent(QMouseEvent* e) {
    if (!(e->buttons() & Qt::LeftButton)) {
        return;
    }

    if ((e->pos() - dragStartPoint).manhattanLength() <
                QApplication::startDragDistance()) {
        return;
    }

    if (!currentItem()) {
        return;
    }

    emit dragRequested(currentId());

    QListWidget::mouseMoveEvent(e);
}
