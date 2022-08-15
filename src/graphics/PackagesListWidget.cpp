#include "PackagesListWidget.h"

#include "../logics/sqlDefines.h"

#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlQuery>

PackagesListWidget::PackagesListWidget(QWidget* parent)
        : QListWidget(parent) {
    setSelectionMode(QListWidget::NoSelection);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &PackagesListWidget::customContextMenuRequested,
            this, &PackagesListWidget::showContextMenu);

    QSqlQuery query;
    LOG_PREPARE(query, " \
        SELECT name, id \
        FROM packages \
    ")
    LOG_EXEC(query)

    while (query.next()) {
        auto* item = new QListWidgetItem(query.value(0).toString());
        item->setData(Qt::UserRole, query.value(1));
        addItem(item);
    }
}

void PackagesListWidget::showContextMenu(const QPoint& pos) {
    auto* curr = currentItem();
    if (!curr) {
        return;
    }

    QMenu menu;

    menu.addAction(tr("Delete"), [=]() {
        if (QMessageBox::question(
                this,
                "Question",
                tr("Delete package \"%1\"?").arg(curr->text()))
                    == QMessageBox::Yes) {
            QSqlQuery query;
            LOG_PREPARE(query, " \
                DELETE \
                FROM packages \
                WHERE id = ? \
            ")
            query.addBindValue(curr->data(Qt::UserRole).toInt());
            LOG_EXEC(query)
        }
    });

    menu.addAction(tr("Rename"), [=]() {
        bool ok;
        auto name = QInputDialog::getText(this, tr("Rename package"),
            tr("Package name:"), QLineEdit::Normal, curr->text(), &ok).trimmed();

        if (ok) {
            QSqlQuery query;
            LOG_PREPARE(query, " \
                UPDATE packages \
                SET name = ? \
                WHERE id = ? \
            ")
            query.addBindValue(name);
            query.addBindValue(curr->data(Qt::UserRole).toInt());
            LOG_EXEC(query)
        }
    });

    menu.exec(mapToGlobal(pos));
}
