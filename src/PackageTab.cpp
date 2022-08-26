#include "PackageTab.h"

#include "sqlDefines.h"
#include "GlobalSignalHandler.h"
#include "PackageInfoDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>

PackageTab::PackageTab(QWidget* parent)
        : QWidget(parent) {
    ui();

    connect(
        this,
        &PackageTab::themesUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::themesUpdated
    );

    connect(
        this,
        &PackageTab::packagesUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::packagesUpdated
    );

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::packagesUpdated,
        this,
        &PackageTab::update
    );

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::themesUpdated,
        this,
        &PackageTab::update
    );

    connect(
        packagesList,
        &SmartListWidget::menuRequested,
        this,
        &PackageTab::packageMenuRequested
    );

    connect(
        packagesList,
        &SmartListWidget::doubleClicked,
        this,
        &PackageTab::packageDoubleClicked
    );

    update();
}

void PackageTab::ui() {
    // Layouts
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    // Create Button
    auto* createBtn = new QPushButton(tr("New package"));
    connect(createBtn, &QPushButton::clicked,
            this, &PackageTab::onCreateBtn);
    hbox->addWidget(createBtn);

    // Import Button
    auto* importBtn = new QPushButton(tr("Import package"));
    hbox->addWidget(importBtn);

    // Packages List
    packagesList = new SmartListWidget;
    vbox->addWidget(packagesList);
}

void PackageTab::onCreateBtn() {
    PackageInfoDialog d(-1, this);
    d.exec();
}

void PackageTab::update() {
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

    packagesList->clear();
    while (query.next()) {
        packagesList->addItem(
            tr("%1 (%2 themes)")
                .arg(query.value(1).toString())
                .arg(query.value(2).toInt()),
            query.value(0).toInt()
        );
    }
}

void PackageTab::packageDoubleClicked(int packageId) {
    PackageInfoDialog d(
        packageId,
        this
    );
    d.exec();
}

void PackageTab::packageMenuRequested(int packageId, const QPoint& globalPos) {
    QMenu menu;

    menu.addAction(tr("Rename"), [=]() {
        PackageInfoDialog d(
            packageId,
            this
        );
        d.exec();
    });

    menu.addAction(tr("Delete"), [=]() {
        if (QMessageBox::question(
                this,
                "Question",
                tr("Delete package \"%1\"?").arg(packageId)) // TODO packageId -> packageName
                    == QMessageBox::Yes) {

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

    menu.exec(globalPos);
}
