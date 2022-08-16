#include "ThemesListWidget.h"
#include "ThemeContextMenu.h"

#include "sqlDefines.h"

#include <QMenu>
#include <QListWidgetItem>
#include <QSqlQuery>

ThemesListWidget::ThemesListWidget(QWidget* parent)
        : QListWidget(parent) {
    setSelectionMode(QListWidget::NoSelection);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &ThemesListWidget::customContextMenuRequested,
            this, &ThemesListWidget::showContextMenu);

    QSqlQuery query;
    LOG_PREPARE(query, " \
        SELECT name, id \
        FROM themes \
    ")
    LOG_EXEC(query)

    while (query.next()) {
        auto* item = new QListWidgetItem(query.value(0).toString());
        item->setData(Qt::UserRole, query.value(1));
        addItem(item);
    }
}

void ThemesListWidget::showContextMenu(const QPoint& pos) {
    // XXX Double right click, when menu is opened
    auto* curr = currentItem();
    if (curr) {
        ThemeContextMenu menu(curr->data(Qt::UserRole).toInt(), curr->text());
        menu.exec(mapToGlobal(pos));
    }
}
