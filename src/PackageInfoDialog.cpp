#include "PackageInfoDialog.h"

#include "GlobalSignalHandler.h"
#include "sqlDefines.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>

PackageInfoDialog::PackageInfoDialog(int packageId, QWidget* parent)
        : QDialog(parent), packageId(packageId) {
    setWindowTitle(tr("Package \"%1\" Info").arg(packageId)); // TODO: packageId -> packageName

    ui();
    load();

    connect(
        this,
        &PackageInfoDialog::packagesUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::packagesUpdated
    );

    connect(
        cancelBtn,
        &QPushButton::pressed,
        this,
        &PackageInfoDialog::reject
    );

    connect(
        saveBtn,
        &QPushButton::pressed,
        this,
        &PackageInfoDialog::accept
    );

    connect(
        this,
        &PackageInfoDialog::accepted,
        this,
        &PackageInfoDialog::save
    );
}

void PackageInfoDialog::ui() {
    // Main Layout
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    // Name
    auto* nameLbl = new QLabel("Package name:");
    vbox->addWidget(nameLbl);

    nameEdit = new QLineEdit;
    vbox->addWidget(nameEdit);

    // Buttons
    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    hbox->addStretch(1);

    cancelBtn = new QPushButton("Cancel");
    hbox->addWidget(cancelBtn);

    saveBtn = new QPushButton(packageId == -1 ? tr("Create") : tr("Update"));
    saveBtn->setDefault(true);
    hbox->addWidget(saveBtn);
}

void PackageInfoDialog::load() {
    if (packageId != -1) {
        PREPARE_NEW(query, " \
            SELECT name \
            FROM packages \
            WHERE id = ? \
        ")
        query.addBindValue(packageId);

        EXEC(query)
        query.first();

        nameEdit->setText(query.value(0).toString());
    }
}

void PackageInfoDialog::save() {
    // Create / update package
    QSqlQuery query;

    if (packageId == -1) {
        PREPARE(query, " \
            INSERT \
            INTO packages(name) \
            VALUES (NULLIF(?, '')) \
        ")
    } else {
        PREPARE(query, " \
            UPDATE packages \
            SET name = NULLIF(?, '') \
            WHERE id = ? \
        ")
    }

    query.addBindValue(nameEdit->text().trimmed());
    if (packageId != -1) {
        query.addBindValue(packageId);
    }

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
                return;
        }
    }

    if (packageId == -1) {
        packageId = query.lastInsertId().toInt();
    }

    emit packagesUpdated();
}
