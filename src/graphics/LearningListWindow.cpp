#include "LearningListWindow.h"
#include "ThemesListWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

LearningListWindow::LearningListWindow(
    const QString& themeName, QWidget* parent)
        : QWidget(parent), themeName(themeName) {
    setWindowTitle(QString("%1 Learning List").arg(themeName));

    // Main Layout
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    // List
    auto* themesList = new ThemesListWidget;
    themesList->addItems({
        "Theme 1",
        "Theme 2",
        "Theme 3",
        "Theme 4",
        "Theme 5",
    });
    vbox->addWidget(themesList);

    // Buttons
    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    hbox->addStretch(1);

    auto* exportBtn = new QPushButton("Export");
    hbox->addWidget(exportBtn);

    auto* okBtn = new QPushButton("Ok");
    hbox->addWidget(okBtn);
}
