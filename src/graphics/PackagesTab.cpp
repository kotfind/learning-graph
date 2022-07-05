#include "PackagesTab.h"

#include "../logics/WorkerCore.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

PackagesTab::PackagesTab(QWidget* parent)
        : QWidget(parent) {
    // Layouts
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    // Create Button
    auto* createBtn = new QPushButton(tr("New package"));
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

    emit listRequested(PackageRequest{
        true, // name
        false
    });
}

void PackagesTab::onListGot(const QVector<Package>& packages) {
    for (const auto& package : packages) {
        packagesList->addItem(package.name);
    }
}
