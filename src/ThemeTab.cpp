#include "ThemeTab.h"

#include "ThemeInfoDialog.h"
#include "db/db.h"
#include "GlobalSignalHandler.h"
#include "ThemeContextMenu.h"
#include "appendExtention.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QDrag>
#include <QMimeData>
#include <QByteArray>
#include <QDataStream>
#include <QStandardPaths>
#include <QFileDialog>

using namespace db;

ThemeTab::ThemeTab(QWidget* parent)
        : QWidget(parent) {
    ui();

    connect(
        createBtn,
        &QPushButton::clicked,
        [this]() {
            (new ThemeInfoDialog(-1, this))->exec();
        }
    );

    connect(
        searchBtn,
        &QPushButton::clicked,
        this,
        &ThemeTab::update
    );

    connect(
        autoUpdateCheck,
        &QCheckBox::stateChanged,
        [this](int state) {
            setAutoUpdate(state == Qt::Checked);
        }
    );

    connect(
        themesList,
        &SmartListWidget::doubleClicked,
        this,
        &ThemeTab::themeDoubleClicked
    );

    connect(
        themesList,
        &SmartListWidget::menuRequested,
        this,
        &ThemeTab::themeMenuRequested
    );

    connect(
        themesList,
        &SmartListWidget::dragRequested,
        this,
        &ThemeTab::themeDragRequested
    );

    connect(
        selectionModeCheck,
        &QCheckBox::stateChanged,
        this,
        &ThemeTab::selectionModeCheckChanged
    );

    connect(
        selectAllButton,
        &QPushButton::pressed,
        this,
        &ThemeTab::selectAllButtonPressed
    );

    connect(
        themesList,
        &SmartListWidget::itemSelectionChanged,
        this,
        &ThemeTab::selectionChanged
    );

    connect(
        exportButton,
        &QPushButton::pressed,
        this,
        &ThemeTab::exportPressed
   );

    autoUpdateCheck->setChecked(true);
    selectionModeCheck->setChecked(false);
    selectAllButton->setDisabled(true);
    exportButton->setDisabled(true);
    update();
}

void ThemeTab::keyPressEvent(QKeyEvent* e) {
    switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            emit update();
            break;
    }
}

void ThemeTab::ui() {
    // Layout
    auto* vbox = new QVBoxLayout;
    setLayout(vbox);

    // Create Button
    createBtn = new QPushButton(tr("New theme"));
    vbox->addWidget(createBtn);

    // Search section
    auto* searchFrame = new QFrame;
    searchFrame->setFrameShape(QFrame::StyledPanel);
    vbox->addWidget(searchFrame);

    auto* grid = new QGridLayout;
    grid->setColumnStretch(0, 0);
    grid->setColumnStretch(1, 1);
    searchFrame->setLayout(grid);

    // Frame Title
    auto* frameLbl = new QLabel(tr("Search section"));
    grid->addWidget(frameLbl, 0, 0, 1, 2, Qt::AlignCenter);

    // Theme
    auto* themeLbl = new QLabel(tr("Theme:"));
    grid->addWidget(themeLbl, 1, 0, Qt::AlignRight);

    nameEdit = new QLineEdit;
    grid->addWidget(nameEdit, 1, 1);

    // Package
    auto* packageLbl = new QLabel(tr("Package:"));
    grid->addWidget(packageLbl, 2, 0, Qt::AlignRight);

    packageCombo = new PackageComboBox;
    packageCombo->setAny(true);
    packageCombo->setCurrent(-1);
    grid->addWidget(packageCombo, 2, 1);

    // In Wishlist Switch
    wishlistCheck = new QCheckBox(tr("In Wishlist"));
    wishlistCheck->setTristate(true);
    wishlistCheck->setCheckState(Qt::PartiallyChecked);
    grid->addWidget(wishlistCheck, 4, 1);

    // Learned List Switch
    learnedCheck = new QCheckBox(tr("Learned"));
    learnedCheck->setTristate(true);
    grid->addWidget(learnedCheck, 3, 1);

    // Search
    searchBtn = new QPushButton(tr("Search"));
    grid->addWidget(searchBtn, 5, 1);

    autoUpdateCheck = new QCheckBox(tr("Auto update"));
    grid->addWidget(autoUpdateCheck, 5, 0);

    // Selection
    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    selectionModeCheck = new QCheckBox(tr("Selection Mode"));
    hbox->addWidget(selectionModeCheck);

    selectAllButton = new QPushButton(tr("Select All"));
    hbox->addWidget(selectAllButton);

    // Themes List
    themesList = new SmartListWidget;
    vbox->addWidget(themesList);

    // Export
    exportButton = new QPushButton(tr("Export"));
    vbox->addWidget(exportButton);
}

void ThemeTab::update() {
    auto themes = theme::reads(
        nameEdit->text().trimmed(),
        packageCombo->currentData().toInt(),
        wishlistCheck->checkState(),
        learnedCheck->checkState()
    );

    themesList->clear();

    for (const auto& t : themes) {
        themesList->addItem(
            tr("%1 @ %2")
                .arg(t.name)
                .arg(t.package.name),
            t.id
        );
    }
}

void ThemeTab::setAutoUpdate(bool state) {
    if (state)  {
        searchBtn->setDisabled(true);

        connect(
            GlobalSignalHandler::getInstance(),
            &GlobalSignalHandler::themesUpdated,
            this,
            &ThemeTab::update
        );

        connect(
            GlobalSignalHandler::getInstance(),
            &GlobalSignalHandler::packagesUpdated,
            this,
            &ThemeTab::update
        );

        connect(
            nameEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeTab::update
        );

        connect(
            packageCombo,
            &PackageComboBox::currentIndexChanged,
            this,
            &ThemeTab::update
        );

        connect(
            wishlistCheck,
            &QCheckBox::stateChanged,
            this,
            &ThemeTab::update
        );

        connect(
            learnedCheck,
            &QCheckBox::stateChanged,
            this,
            &ThemeTab::update
        );

        update();
    } else {
        searchBtn->setDisabled(false);

        disconnect(
            GlobalSignalHandler::getInstance(),
            &GlobalSignalHandler::themesUpdated,
            this,
            &ThemeTab::update
        );

        disconnect(
            GlobalSignalHandler::getInstance(),
            &GlobalSignalHandler::packagesUpdated,
            this,
            &ThemeTab::update
        );


        disconnect(
            nameEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeTab::update
        );

        disconnect(
            packageCombo,
            &PackageComboBox::currentIndexChanged,
            this,
            &ThemeTab::update
        );

        disconnect(
            wishlistCheck,
            &QCheckBox::stateChanged,
            this,
            &ThemeTab::update
        );

        disconnect(
            learnedCheck,
            &QCheckBox::stateChanged,
            this,
            &ThemeTab::update
        );
    }
}

void ThemeTab::themeDoubleClicked(int themeId) {
    (new ThemeInfoDialog(themeId, this))->exec();
}

void ThemeTab::themeMenuRequested(int themeId, const QPoint& globalPos) {
    ThemeContextMenu menu(themeId, this);
    menu.exec(globalPos);
}

void ThemeTab::themeDragRequested(int themeId) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << themeId;

    auto* mimeData = new QMimeData;
    mimeData->setData("application/x-themeid", data);

    auto* drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction);
}

void ThemeTab::selectionModeCheckChanged(int state) {
    if (state == Qt::Checked) {
        themesList->setSelectionMode(true);
        selectAllButton->setDisabled(false);
    } else {
        themesList->setSelectionMode(false);
        selectAllButton->setDisabled(true);
    }
}

void ThemeTab::selectAllButtonPressed() {
    if (themesList->selectedItems().empty()) {
        themesList->selectAll();
    } else {
        themesList->clearSelection();
    }
}

void ThemeTab::selectionChanged() {
    if (themesList->selectedItems().empty()) {
        selectAllButton->setText(tr("Select All"));
        exportButton->setDisabled(true);
    } else {
        selectAllButton->setText(tr("Clear"));
        exportButton->setDisabled(false);
    }
}

void ThemeTab::exportPressed() {
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
    theme::exportAsTxt(filename, themesList->getSelectedIds());
}
