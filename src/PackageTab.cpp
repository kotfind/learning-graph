#include "PackageTab.h"

#include "db/db.h"
#include "GlobalSignalHandler.h"
#include "PackageInfoDialog.h"
#include "appendExtention.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include <QLabel>
#include <QGridLayout>
#include <QStandardPaths>
#include <QFileDialog>

using namespace db;

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
        packagesList,
        &SmartListWidget::itemMenuRequested,
        this,
        &PackageTab::onPackageMenuRequested
    );

    connect(
        packagesList,
        &SmartListWidget::itemDoubleClicked,
        this,
        &PackageTab::onPackageDoubleClicked
    );

    connect(
        updateButton,
        &QPushButton::pressed,
        this,
        &PackageTab::update
    );

    connect(
        autoUpdateCheckBox,
        &QCheckBox::stateChanged,
        [this](int state) {
            setAutoUpdate(state == Qt::Checked);
        }
    );

    connect(
        selectAllButton,
        &QPushButton::pressed,
        this,
        &PackageTab::onSelectAllButtonPressed
    );

    connect(
        packagesList,
        &SmartListWidget::itemSelectionChanged,
        this,
        &PackageTab::onSelectionChanged
    );

    connect(
        exportButton,
        &QPushButton::pressed,
        this,
        &PackageTab::onExportButtonPressed
   );

    update();
    setAutoUpdate(true);
    autoUpdateCheckBox->setChecked(true);
    packagesList->setSelectionMode(true);
    exportButton->setDisabled(true);
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

    // Search section
    auto* searchFrame = new QFrame;
    searchFrame->setFrameShape(QFrame::StyledPanel);
    vbox->addWidget(searchFrame);

    auto* grid = new QGridLayout;
    grid->setColumnStretch(0, 0);
    grid->setColumnStretch(1, 1);
    searchFrame->setLayout(grid);

    grid->addWidget(
        new QLabel(tr("Search Section")),
        0, 0,
        1, 2,
        Qt::AlignCenter
    );

    grid->addWidget(
        new QLabel(tr("Name:")),
        1, 0,
        Qt::AlignRight
    );

    nameEdit = new QLineEdit;
    grid->addWidget(nameEdit, 1, 1);

    autoUpdateCheckBox = new QCheckBox(tr("Auto update"));
    grid->addWidget(
        autoUpdateCheckBox,
        2, 0
    );

    updateButton = new QPushButton(tr("Search"));
    grid->addWidget(
        updateButton,
        2, 1
    );

    // Selection
    selectAllButton = new QPushButton(tr("Select All"));
    vbox->addWidget(selectAllButton);

    // Packages List
    packagesList = new SmartListWidget;
    vbox->addWidget(packagesList);

    // Export
    exportButton = new QPushButton(tr("Export"));
    vbox->addWidget(exportButton);
}

void PackageTab::onCreateBtn() {
    PackageInfoDialog d(-1, this);
    d.exec();
}

void PackageTab::update() {
    auto packages = package::reads(nameEdit->text().trimmed());

    packagesList->clear();
    for (const auto& p : packages) {
        packagesList->addItem(
            tr("%1 (%2 themes)")
                .arg(p.name)
                .arg(p.count),
            p.id
        );
    }
}

void PackageTab::onPackageDoubleClicked(int packageId) {
    PackageInfoDialog d(
        packageId,
        this
    );
    d.exec();
}

void PackageTab::onPackageMenuRequested(int packageId, const QPoint& globalPos) {
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
                tr("Delete package \"%1\"?").arg(package::name(packageId)))
                    == QMessageBox::Yes) {

            package::del(packageId);

            emit packagesUpdated();
            emit themesUpdated();
        }
    });

    menu.exec(globalPos);
}

void PackageTab::setAutoUpdate(bool state) {
    if (state) {
        updateButton->setDisabled(true);

        connect(
            nameEdit,
            &QLineEdit::textChanged,
            this,
            &PackageTab::update
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

        update();
    } else {
        updateButton->setDisabled(false);

        disconnect(
            nameEdit,
            &QLineEdit::textChanged,
            this,
            &PackageTab::update
        );

        disconnect(
            GlobalSignalHandler::getInstance(),
            &GlobalSignalHandler::packagesUpdated,
            this,
            &PackageTab::update
        );

        disconnect(
            GlobalSignalHandler::getInstance(),
            &GlobalSignalHandler::themesUpdated,
            this,
            &PackageTab::update
        );
    }
}

void PackageTab::onSelectAllButtonPressed() {
    if (packagesList->selectedItems().empty()) {
        packagesList->selectAll();
    } else {
        packagesList->clearSelection();
    }
}

void PackageTab::onSelectionChanged() {
    if (packagesList->selectedItems().empty()) {
        selectAllButton->setText(tr("Select All"));
        exportButton->setDisabled(true);
    } else {
        selectAllButton->setText(tr("Clear"));
        exportButton->setDisabled(false);
    }
}

void PackageTab::onExportButtonPressed() {
    const QString txtFilter = tr("Text (*.txt)");

    QString selectedFilter;
    auto filename = QFileDialog::getSaveFileName(
        this,
        tr("Export to ..."),
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
        txtFilter,
        &selectedFilter
    );

    if (filename.isEmpty()) {
        return;
    }

    appendExtentionIfNot(filename, ".txt");
    package::exportAsTxt(filename, packagesList->getSelectedIds());
}
