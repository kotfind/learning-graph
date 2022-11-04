#include "LearningListWidget.h"

#include "GlobalSignalHandler.h"
#include "db/db.h"
#include "ThemeContextMenu.h"
#include "appendExtention.h"
#include "filesystem/filesystem.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>

using namespace db;

LearningListWidget::LearningListWidget(QWidget* parent)
        : QWidget(parent) {
    ui();

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::openList,
        this,
        &LearningListWidget::open
    );

    connect(
        closeButton,
        &QPushButton::pressed,
        this,
        &LearningListWidget::close
    );

    connect(
        themesList,
        &SmartListWidget::itemMenuRequested,
        this,
        &LearningListWidget::onThemeMenuRequested
    );

    connect(
        exportButton,
        &QPushButton::clicked,
        this,
        &LearningListWidget::onExportButtonClicked
    );

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::themesUpdated,
        this,
        &LearningListWidget::onThemesUpdated
    );

    connect(
        showLearnedCheckBox,
        &QCheckBox::stateChanged,
        this,
        &LearningListWidget::onShowLearnedCheckBoxChanged
    );

    showLearnedCheckBox->setChecked(true);
    setDisabled(true);

    load();
}

void LearningListWidget::ui() {
    // Main Layout
    auto* vbox = new QVBoxLayout;
    setLayout(vbox);

    // Title
    mainLabel = new QLabel(tr("No list loaded"));
    vbox->addWidget(mainLabel);

    // Show Learned CheckBox
    showLearnedCheckBox = new QCheckBox(tr("Show learned themes"));
    vbox->addWidget(showLearnedCheckBox);

    // List
    themesList = new SmartListWidget;
    themesList->setSelectionMode(false);
    vbox->addWidget(themesList);

    // Buttons
    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    hbox->addStretch(1);

    exportButton = new QPushButton(tr("Export"));
    hbox->addWidget(exportButton);

    closeButton = new QPushButton(tr("Close"));
    hbox->addWidget(closeButton);
}

void LearningListWidget::open(int themeId) {
    try {
        list::build(themeId);
    } catch (const QString& msg) {
        QMessageBox::critical(
            this,
            tr("Error"),
            msg
        );
        return;
    }

    load();
}

void LearningListWidget::load() {
    if (db::list::empty()) {
        return;
    }

    auto themes = list::reads(
        showLearnedCheckBox->checkState() == Qt::Unchecked
    );

    auto mainThemeId = db::list::getMainThemeId();
    mainLabel->setText(tr("List for theme \"%1 (%2)\"")
        .arg(db::theme::name(mainThemeId))
        .arg(db::theme::packageName(mainThemeId))
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

    setDisabled(false);
}

void LearningListWidget::close() {
    mainLabel->setText(tr("No list loaded"));

    themesList->clear();
    list::clear();

    showLearnedCheckBox->setChecked(true);
    setDisabled(true);
}

void LearningListWidget::onThemeMenuRequested(int themeId, const QPoint& globalPos) {
    ThemeContextMenu menu(themeId, this);
    menu.exec(globalPos);
}

void LearningListWidget::onExportButtonClicked() {
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
    filesystem::list::exportAsTxt(filename);
}

void LearningListWidget::onThemesUpdated() {
    if (db::list::empty() || !db::theme::exists(db::list::getMainThemeId())) {
        close();
    } else {
        db::list::deleteDeletedThemes();
        load();
    }
}

void LearningListWidget::onShowLearnedCheckBoxChanged(int state) {
    load();
}
