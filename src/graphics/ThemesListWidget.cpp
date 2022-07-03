#include "ThemesListWidget.h"
#include "ThemeContextMenuWidget.h"

#include "../logics/WorkerCore.h"

#include <QMenu>
#include <QListWidgetItem>

ThemesListWidget::ThemesListWidget(QWidget* parent)
        : QListWidget(parent) {
    setSelectionMode(QListWidget::NoSelection);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &ThemesListWidget::customContextMenuRequested,
            this, &ThemesListWidget::showContextMenu);

    connect(WorkerCore::getInstance(), &WorkerCore::themesListGot,
            this, &ThemesListWidget::onListGot);
}

void ThemesListWidget::showContextMenu(const QPoint& pos) {
    // XXX Double right click, when menu is opened
    ThemeContextMenuWidget menu(currentItem()->data(Qt::UserRole).toInt());
    menu.exec(mapToGlobal(pos));
}

void ThemesListWidget::onListGot(const QVector<Theme>& themes) {
    clear();
    for (const auto& theme : themes) {
        auto* item = new QListWidgetItem(theme.name);
        item->setData(Qt::UserRole, theme.id);
        addItem(item);
    }
}
