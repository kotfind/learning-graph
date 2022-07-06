#include "PackagesTab.h"

#include "../logics/WorkerCore.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QInputDialog>

PackagesTab::PackagesTab(QWidget* parent)
        : QWidget(parent) {
    // Layouts
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    // Create Button
    auto* createBtn = new QPushButton(tr("New package"));
    connect(createBtn, &QPushButton::clicked,
            this, &PackagesTab::onCreateBtn);
    hbox->addWidget(createBtn);

    // Import Button
    auto* importBtn = new QPushButton(tr("Import package"));
    hbox->addWidget(importBtn);

    // Packages List
    packagesList = new QListWidget;
    vbox->addWidget(packagesList);

    // Connections

    connect(WorkerCore::getInstance(), &WorkerCore::packagesListGot,
            this, &PackagesTab::onListGot);
    connect(this, &PackagesTab::listRequested,
            WorkerCore::getInstance(), &WorkerCore::getPackagesList);
    connect(this, &PackagesTab::creationRequested,
            WorkerCore::getInstance(), &WorkerCore::createPackage);
    connect(WorkerCore::getInstance(), &WorkerCore::packagesChanged,
            this, &PackagesTab::listRequested);

    emit listRequested();
}

void PackagesTab::onListGot(const QVector<Package>& packages) {
    packagesList->clear();
    for (const auto& package : packages) {
        packagesList->addItem(package.name);
    }
}

void PackagesTab::onCreateBtn() {
    bool ok;
    QString name = QInputDialog::getText(this, tr("New package"),
        tr("New package name:"), QLineEdit::Normal, "", &ok).trimmed();
    if (ok) {
        emit creationRequested(Package{-1, name});
    }
}
