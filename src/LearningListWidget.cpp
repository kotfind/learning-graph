#include "LearningListWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

LearningListWidget::LearningListWidget(QWidget* parent)
        : QWidget(parent) {
    ui();

    themesList->setDisabled(true);
    exportButton->setDisabled(true);
    closeButton->setDisabled(true);
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
