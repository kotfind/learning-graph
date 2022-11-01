#include "PackageInfoDialog.h"

#include "GlobalSignalHandler.h"
#include "db/db.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>

using namespace db;

PackageInfoDialog::PackageInfoDialog(int packageId, QWidget* parent)
        : QDialog(parent), packageId(packageId) {
    setWindowTitle(packageId != -1
        ? tr("Package \"%1\" Info").arg(package::name(packageId))
        : tr("New Package")
    );

    ui();
    load();

    connect(
        this,
        &PackageInfoDialog::packagesUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::packagesUpdated
    );

    connect(
        cancelButton,
        &QPushButton::pressed,
        this,
        &PackageInfoDialog::reject
    );

    connect(
        saveButton,
        &QPushButton::pressed,
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

    cancelButton = new QPushButton("Cancel");
    hbox->addWidget(cancelButton);

    saveButton = new QPushButton(packageId == -1 ? tr("Create") : tr("Update"));
    saveButton->setDefault(true);
    hbox->addWidget(saveButton);
}

void PackageInfoDialog::load() {
    if (packageId != -1) {
        nameEdit->setText(package::name(packageId));
    }
}

void PackageInfoDialog::save() {
    try {
        Package p;
        p.id = packageId;
        p.name = nameEdit->text().trimmed();
        packageId = package::write(p);
    } catch (const QString& msg) {
        QMessageBox::critical(
            this,
            tr("Error"),
            msg
        );
        return;
    } catch (...) {
        return;
    }

    emit packagesUpdated();
    emit accept();
}
