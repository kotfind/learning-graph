#include "SmartListWidget.h"

#include <QListWidgetItem>
#include <QApplication>
#include <QDebug>

SmartListWidget::SmartListWidget(QWidget* parent) 
        : QListWidget(parent) {
    setContextMenuPolicy(Qt::DefaultContextMenu);
    setSelectionMode(false);
}

void SmartListWidget::addItem(const QString& name, int id) {
    auto* item = new QListWidgetItem(name);
    item->setData(Qt::UserRole, id);
    QListWidget::addItem(item);
}

int SmartListWidget::currentId() const {
    return currentItem()->data(Qt::UserRole).toInt();
}

void SmartListWidget::mouseDoubleClickEvent(QMouseEvent* e) {
    if ((e->buttons() & Qt::LeftButton) && currentItem()) {
        emit doubleClicked(currentId());
    }
}

void SmartListWidget::contextMenuEvent(QContextMenuEvent* e) {
    if (currentItem()) {
        emit menuRequested(currentId(), e->globalPos());
    }
}

void SmartListWidget::mousePressEvent(QMouseEvent* e) {
    if (!selectionMode) {
        if (e->buttons() & Qt::LeftButton) {
            dragStartPoint = e->pos();
        }
    }

    QListWidget::mousePressEvent(e);
}

void SmartListWidget::mouseMoveEvent(QMouseEvent* e) {
    if (!selectionMode) {
        if ((e->buttons() & Qt::LeftButton) &&
                (e->pos() - dragStartPoint).manhattanLength() >=
                    QApplication::startDragDistance() &&
                currentItem()
                ) {
            emit dragRequested(currentId());
        }
    }

    QListWidget::mouseMoveEvent(e);
}

void SmartListWidget::setSelectionMode(bool v) {
    selectionMode = v;
    QListWidget::setSelectionMode(selectionMode
        ? QAbstractItemView::MultiSelection
        : QAbstractItemView::NoSelection
    );
}

QList<int> SmartListWidget::getSelectedIds() const {
    QList<int> ans;
    ans.reserve(selectedItems().size());
    for (auto* item : selectedItems()) {
        ans << item->data(Qt::UserRole).toInt();
    }
    return ans;
}
