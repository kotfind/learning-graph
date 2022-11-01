#include "LearningListWidget.h"

#include "GlobalSignalHandler.h"
#include "db/db.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

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

    setDisabled(true);
}

void LearningListWidget::ui() {
    // Main Layout
    auto* vbox = new QVBoxLayout;
    setLayout(vbox);

    // Title
    mainLabel = new QLabel(tr("No list loaded"));
    vbox->addWidget(mainLabel);

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
    mainLabel->setText(tr("List for theme %1").arg(theme::name(themeId)));

    list::build(themeId);
    auto themes = list::reads();

    themesList->clear();
    for (const auto& t : themes) {
        themesList->addItem(
            tr("%1 from %2")
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

    setDisabled(true);
}
