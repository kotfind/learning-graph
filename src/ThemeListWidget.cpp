#include "ThemeListWidget.h"
#include "ThemeContextMenu.h"

#include <QMenu>
#include <QListWidgetItem>

ThemeListWidget::ThemeListWidget(QWidget* parent)
        : QListWidget(parent) {
    setSelectionMode(QListWidget::NoSelection);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &ThemeListWidget::customContextMenuRequested,
            this, &ThemeListWidget::showContextMenu);
}

void ThemeListWidget::showContextMenu(const QPoint& pos) {
    // XXX Double right click, when menu is opened
    auto* curr = currentItem();
    if (curr) {
        ThemeContextMenu menu(curr->data(Qt::UserRole).toInt(), curr->text());
        menu.exec(mapToGlobal(pos));
    }
}
