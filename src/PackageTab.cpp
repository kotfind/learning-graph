#include "PackageTab.h"

#include "db/db.h"
#include "GlobalSignalHandler.h"
#include "PackageInfoDialog.h"
#include "appendExtention.h"
#include "filesystem/filesystem.h"
#include "GenerationOptionsDialog.h"

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
#include <qmessagebox.h>

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
        &QPushButton::clicked,
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
        &QPushButton::clicked,
        this,
        &PackageTab::onSelectAllButtonClicked
    );

    connect(
        packagesList,
        &SmartListWidget::itemSelectionChanged,
        this,
        &PackageTab::onSelectionChanged
    );

    connect(
        exportButton,
        &QPushButton::clicked,
        this,
        &PackageTab::onExportButtonClicked
    );

    connect(
        importButton,
        &QPushButton::clicked,
        this,
        &PackageTab::onImportButtonClicked
    );

    connect(
        createEmptyPackageAction,
        &QAction::triggered,
        this,
        &PackageTab::onCreateEmptyPackageActionTriggered
    );

    connect(
        generatePackageAction,
        &QAction::triggered,
        this,
        &PackageTab::onGeneratePackageActionTriggered
    );

    // Package Generator
    packageGenerator = new PackageGenerator(this);

    connect(
        packageGenerator,
        &PackageGenerator::dependencyDirectionQuestionRequested,
        this,
        &PackageTab::onEdgeDirectionQuestionRequested
    );

    connect(
        this,
        &PackageTab::dirrectionQuestionReplied,
        packageGenerator,
        &PackageGenerator::onDirrectionReplied
    );

    connect(
        packageGenerator,
        &PackageGenerator::done,
        this,
        &PackageTab::onGenerationDone
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
    createButton = new QPushButton(tr("New package"));
    hbox->addWidget(createButton);

    // Import Button
    importButton = new QPushButton(tr("Import package"));
    hbox->addWidget(importButton);

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

    autoUpdateCheckBox = new QCheckBox(tr("Autoupdate"));
    grid->addWidget(
        autoUpdateCheckBox,
        2, 0
    );

    updateButton = new QPushButton(tr("Search"));
    grid->addWidget(
        updateButton,
        2, 1,
        1, 1,
        Qt::AlignRight
    );

    // Selection
    selectAllButton = new QPushButton(tr("Select All"));
    vbox->addWidget(selectAllButton, 0, Qt::AlignRight);

    // Packages List
    packagesList = new SmartListWidget;
    vbox->addWidget(packagesList);

    // Export
    exportButton = new QPushButton(tr("Export"));
    vbox->addWidget(exportButton, 0, Qt::AlignHCenter);

    // Create Menu
    createMenu = new QMenu(createButton);
    createButton->setMenu(createMenu);

    createEmptyPackageAction = new QAction(tr("Create Empty Package"), createMenu);
    createMenu->addAction(createEmptyPackageAction);

    generatePackageAction = new QAction(tr("Generate Package"), createMenu);
    createMenu->addAction(generatePackageAction);
}

void PackageTab::update() {
    auto packages = db::package::reads(nameEdit->text().trimmed());

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
                tr("Question"),
                tr("Delete package \"%1\"?").arg(db::package::name(packageId)))
                    == QMessageBox::Yes) {

            db::package::del(packageId);

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

void PackageTab::onSelectAllButtonClicked() {
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

void PackageTab::onExportButtonClicked() {
    const QString txtFilter = tr("Text file (*.txt)");
    const QString packFilter = tr("Learning Graph packages (*.pack)");
    QString selectedFilter;
    auto filename = QFileDialog::getSaveFileName(
        this,
        tr("Export to ..."),
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
        txtFilter + ";;" + packFilter,
        &selectedFilter
    );

    if (filename.isEmpty()) {
        return;
    }

    if (selectedFilter == txtFilter) {
        appendExtentionIfNot(filename, ".txt");
        filesystem::package::exportAsTxt(filename, packagesList->getSelectedIds());
    } else {
        appendExtentionIfNot(filename, ".pack");
        filesystem::package::exportAsPack(filename, packagesList->getSelectedIds());
    }
}

void PackageTab::onImportButtonClicked() {
    const QString packFilter = tr("Learning Graph packages (*.pack)");
    auto filename = QFileDialog::getOpenFileName(
        this,
        tr("Import from ..."),
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
        packFilter
    );

    if (filename.isEmpty()) {
        return;
    }

    try {
        filesystem::package::importFromPack(filename);
    } catch (const QString& msg) {
        QMessageBox::critical(
            this,
            tr("Error"),
            msg
        );
        return;
    }

    emit packagesUpdated();
}

void PackageTab::onCreateEmptyPackageActionTriggered() {
    PackageInfoDialog d(-1, this);
    d.exec();
}

void PackageTab::onGeneratePackageActionTriggered() {
    PackageInfoDialog packageDialog(-1, this);
    if (packageDialog.exec() == QDialog::Rejected) {
        return;
    }
    int packageId = packageDialog.getId();

    GenerationOptionsDialog optionsDialog(packageId, this);
    if (optionsDialog.exec() == QDialog::Rejected) {
        db::package::del(packageId);
        emit packagesUpdated();
        return;
    }

    packageGenerator->exec(
        packageId,
        optionsDialog.getName(),
        optionsDialog.getDepthLimit(),
        optionsDialog.getQuantityLimit()
    );
}

void PackageTab::onEdgeDirectionQuestionRequested(
        const QString& first,
        const QString& second
    ) {
    qDebug() << "Reply";
    emit dirrectionQuestionReplied(CANCEL_DIRECTION);
}

void PackageTab::onGenerationDone() {
    QMessageBox::information(
        this,
        tr("Graph Generation Done"),
        tr("Graph Generation Done")
    );

    emit themesUpdated();
}
