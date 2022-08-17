#include "PackageListWidget.h"

#include "sqlDefines.h"
#include "GlobalSignalHandler.h"

#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlQuery>

PackageListWidget::PackageListWidget(QWidget* parent)
        : QListWidget(parent) {
    setSelectionMode(QListWidget::NoSelection);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(
        this,
        &PackageListWidget::customContextMenuRequested,
        this,
        &PackageListWidget::showContextMenu
    );

    connect(
        this,
        &PackageListWidget::packagesUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::packagesUpdated
    );

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::packagesUpdated,
        this,
        &PackageListWidget::update
    );

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::themesUpdated,
        this,
        &PackageListWidget::update
    );
}

void PackageListWidget::update() {
    clear();

    QSqlQuery query;
    LOG_PREPARE(query, " \
        SELECT p.id, p.name, ( \
            SELECT COUNT(*) \
            FROM themes t \
            WHERE t.package_id = p.id \
        ) \
        FROM packages p \
    ")
    LOG_EXEC(query)

    while (query.next()) {
        auto name = tr("%1 (%2 themes)")
            .arg(query.value(1).toString())
            .arg(query.value(2).toInt());
        auto* item = new QListWidgetItem(name);
        item->setData(Qt::UserRole, query.value(0));
        addItem(item);
    }
}

void PackageListWidget::showContextMenu(const QPoint& pos) {
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

            emit packagesUpdated();
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

            emit packagesUpdated();
        }
    });

    menu.exec(mapToGlobal(pos));
}
