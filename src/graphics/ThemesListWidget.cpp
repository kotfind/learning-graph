#include "ThemesListWidget.h"
#include "ThemeContextMenuWidget.h"

#include <QMenu>

ThemesListWidget::ThemesListWidget(QWidget* parent)
        : QListWidget(parent) {
    setSelectionMode(QListWidget::NoSelection);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &ThemesListWidget::customContextMenuRequested,
            this, &ThemesListWidget::showContextMenu);
}

void ThemesListWidget::showContextMenu(const QPoint& pos) {
    // XXX Double right click, when menu is opened
    ThemeContextMenuWidget menu(currentItem()->text());
    menu.exec(mapToGlobal(pos));
}
