#include "PackageListWidget.h"

#include "sqlDefines.h"
#include "GlobalSignalHandler.h"
#include "PackageInfoDialog.h"

#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>

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
        this,
        &PackageListWidget::themesUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::themesUpdated
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

    PREPARE_NEW(query, " \
        SELECT p.id, p.name, ( \
            SELECT COUNT(*) \
            FROM themes t \
            WHERE t.packageId = p.id \
        ) \
        FROM packages p \
        ORDER BY p.name \
    ")
    EXEC(query)

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

            auto packageId = curr->data(Qt::UserRole).toInt();

            QSqlQuery query;

            PREPARE(query, " \
                DELETE \
                FROM packages \
                WHERE id = ? \
            ")
            query.addBindValue(packageId);
            EXEC(query)
            query.finish();

            PREPARE(query, " \
                DELETE \
                FROM themes \
                WHERE packageId = ? \
            ")
            query.addBindValue(packageId);
            EXEC(query)

            emit packagesUpdated();
            emit themesUpdated();
        }
    });

    menu.addAction(tr("Rename"), [=]() {
        PackageInfoDialog d(
            curr->data(Qt::UserRole).toInt(),
            this
        );
        d.exec();
    });

    menu.exec(mapToGlobal(pos));
}
