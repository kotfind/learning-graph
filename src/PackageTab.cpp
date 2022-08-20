#include "PackageTab.h"

#include "sqlDefines.h"
#include "PackageListWidget.h"
#include "GlobalSignalHandler.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QSqlQuery>
#include <QMessageBox>

PackageTab::PackageTab(QWidget* parent)
        : QWidget(parent) {
    ui();

    connect(
        this,
        &PackageTab::packagesUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::packagesUpdated
    );
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
    auto* packagesList = new PackageListWidget;
    packagesList->update();
    vbox->addWidget(packagesList);
}

void PackageTab::onCreateBtn() {
    bool ok;
    auto name = QInputDialog::getText(this, tr("New package"),
        tr("New package name:"), QLineEdit::Normal, "", &ok).trimmed();
    if (ok) {
        // Add package
        QSqlQuery query;
        LOG_PREPARE(query, " \
            INSERT \
            INTO packages(name) \
            VALUES (NULLIF(?, '')) \
        ")
        query.addBindValue(name);

        if (!query.exec()) {
            switch(ERR_CODE(query)) {
                case SQLITE_CONSTRAINT_UNIQUE:
                    QMessageBox::critical(
                        this,
                        tr("Error"),
                        tr("Name is not unique.")
                    );
                    return;

                case SQLITE_CONSTRAINT_NOTNULL:
                    QMessageBox::critical(
                        this,
                        tr("Error"),
                        tr("Name should not be empty.")
                    );
                    return;

                default:
                    LOG_FAILED_QUERY(query);
                    break;
            }
        }
        emit packagesUpdated();
    }
}
