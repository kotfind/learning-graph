#include "PackagesTab.h"

#include "../logics/WorkerCore.h"
#include "PackagesListWidget.h"

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
    auto* packagesList = new PackagesListWidget;
    vbox->addWidget(packagesList);

    // Connections

    connect(this, &PackagesTab::create,
            WorkerCore::getInstance(), &WorkerCore::createPackage);

    emit packagesList->getList();
}

void PackagesTab::onCreateBtn() {
    bool ok;
    auto name = QInputDialog::getText(this, tr("New package"),
        tr("New package name:"), QLineEdit::Normal, "", &ok).trimmed();
    if (ok) {
        emit create(Package{-1, name});
    }
}
