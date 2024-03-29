#include "ThemeTab.h"

#include "ThemeInfoDialog.h"
#include "db/db.h"
#include "GlobalSignalHandler.h"
#include "ThemeContextMenu.h"
#include "appendExtention.h"
#include "filesystem/filesystem.h"

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

ThemeTab::ThemeTab(QWidget* parent)
        : QWidget(parent) {
    ui();

    connect(
        createButton,
        &QPushButton::clicked,
        [this]() {
            (new ThemeInfoDialog(-1, this))->exec();
        }
    );

    connect(
        searchButton,
        &QPushButton::clicked,
        this,
        &ThemeTab::update
    );

    connect(
        autoUpdateCheckBox,
        &QCheckBox::stateChanged,
        [this](int state) {
            setAutoUpdate(state == Qt::Checked);
        }
    );

    connect(
        themesList,
        &SmartListWidget::itemDoubleClicked,
        this,
        &ThemeTab::onThemeDoubleClicked
    );

    connect(
        themesList,
        &SmartListWidget::itemMenuRequested,
        this,
        &ThemeTab::onThemeMenuRequested
    );

    connect(
        themesList,
        &SmartListWidget::itemDragRequested,
        this,
        &ThemeTab::onThemeDragRequested
    );

    connect(
        selectionModeCheckBox,
        &QCheckBox::stateChanged,
        this,
        &ThemeTab::onSelectionModeCheckChanged
    );

    connect(
        selectAllButton,
        &QPushButton::clicked,
        this,
        &ThemeTab::onSelectAllButtonClicked
    );

    connect(
        themesList,
        &SmartListWidget::itemSelectionChanged,
        this,
        &ThemeTab::onSelectionChanged
    );

    connect(
        exportButton,
        &QPushButton::clicked,
        this,
        &ThemeTab::onExportButtonClicked
   );

    autoUpdateCheckBox->setChecked(true);
    selectionModeCheckBox->setChecked(false);
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
    createButton = new QPushButton(tr("New theme"));
    vbox->addWidget(createButton, 0, Qt::AlignRight);

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

    packageComboBox = new PackageComboBox;
    packageComboBox->setAny(true);
    packageComboBox->setCurrentId(-1);
    grid->addWidget(packageComboBox, 2, 1);

    // In Wishlist Switch
    wishlistCheckBox = new QCheckBox(tr("In Wishlist"));
    wishlistCheckBox->setTristate(true);
    wishlistCheckBox->setCheckState(Qt::PartiallyChecked);
    grid->addWidget(wishlistCheckBox, 4, 1);

    // Learned List Switch
    learnedCheckBox = new QCheckBox(tr("Learned"));
    learnedCheckBox->setTristate(true);
    learnedCheckBox->setCheckState(Qt::PartiallyChecked);
    grid->addWidget(learnedCheckBox, 3, 1);

    // Search
    searchButton = new QPushButton(tr("Search"));
    grid->addWidget(searchButton, 5, 1, 1, 1, Qt::AlignRight);

    autoUpdateCheckBox = new QCheckBox(tr("Autoupdate"));
    grid->addWidget(autoUpdateCheckBox, 5, 0);

    // Selection
    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    selectionModeCheckBox = new QCheckBox(tr("Selection Mode"));
    hbox->addWidget(selectionModeCheckBox);

    selectAllButton = new QPushButton(tr("Select All"));
    hbox->addWidget(selectAllButton, 0, Qt::AlignRight);

    // Themes List
    themesList = new SmartListWidget;
    vbox->addWidget(themesList);

    // Export
    exportButton = new QPushButton(tr("Export"));
    vbox->addWidget(exportButton, 0, Qt::AlignHCenter);
}

void ThemeTab::update() {
    auto themes = db::theme::reads(
        nameEdit->text().trimmed(),
        packageComboBox->currentData().toInt(),
        wishlistCheckBox->checkState(),
        learnedCheckBox->checkState()
    );

    themesList->clear();

    for (const auto& t : themes) {
        themesList->addItem(
            tr("%1 (%2)")
                .arg(t.name)
                .arg(t.package.name),
            t.id
        );
    }
}

void ThemeTab::setAutoUpdate(bool state) {
    if (state)  {
        searchButton->setDisabled(true);

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
            packageComboBox,
            &PackageComboBox::currentIndexChanged,
            this,
            &ThemeTab::update
        );

        connect(
            wishlistCheckBox,
            &QCheckBox::stateChanged,
            this,
            &ThemeTab::update
        );

        connect(
            learnedCheckBox,
            &QCheckBox::stateChanged,
            this,
            &ThemeTab::update
        );

        update();
    } else {
        searchButton->setDisabled(false);

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
            packageComboBox,
            &PackageComboBox::currentIndexChanged,
            this,
            &ThemeTab::update
        );

        disconnect(
            wishlistCheckBox,
            &QCheckBox::stateChanged,
            this,
            &ThemeTab::update
        );

        disconnect(
            learnedCheckBox,
            &QCheckBox::stateChanged,
            this,
            &ThemeTab::update
        );
    }
}

void ThemeTab::onThemeDoubleClicked(int themeId) {
    (new ThemeInfoDialog(themeId, this))->exec();
}

void ThemeTab::onThemeMenuRequested(int themeId, const QPoint& globalPos) {
    ThemeContextMenu menu(themeId, this);
    menu.exec(globalPos);
}

void ThemeTab::onThemeDragRequested(int themeId) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << themeId;

    auto* mimeData = new QMimeData;
    mimeData->setData("application/x-themeid", data);

    auto* drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction);
}

void ThemeTab::onSelectionModeCheckChanged(int state) {
    if (state == Qt::Checked) {
        themesList->setSelectionMode(true);
        selectAllButton->setDisabled(false);
    } else {
        themesList->setSelectionMode(false);
        selectAllButton->setDisabled(true);
    }
}

void ThemeTab::onSelectAllButtonClicked() {
    if (themesList->selectedItems().empty()) {
        themesList->selectAll();
    } else {
        themesList->clearSelection();
    }
}

void ThemeTab::onSelectionChanged() {
    if (themesList->selectedItems().empty()) {
        selectAllButton->setText(tr("Select All"));
        exportButton->setDisabled(true);
    } else {
        selectAllButton->setText(tr("Clear"));
        exportButton->setDisabled(false);
    }
}

void ThemeTab::onExportButtonClicked() {
    const QString txtFilter = tr("Text file (*.txt)");

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
    filesystem::theme::exportAsTxt(filename, themesList->getSelectedIds());
}
