#include "LearningListWidget.h"

#include "GlobalSignalHandler.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <qpushbutton.h>

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
    mainLabel = new QLabel(tr("No List loaded"));
    vbox->addWidget(mainLabel);

    // List
    themesList = new SmartListWidget;
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
    setDisabled(false);
}

void LearningListWidget::close() {
    setDisabled(true);
}
