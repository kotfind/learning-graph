#include "GraphEditTab.h"

#include "GraphNodeWidget.h"

#include <QWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QIcon>
#include <QLabel>
#include <QSpinBox>

GraphEditTab::GraphEditTab(QWidget* parent)
        : QMainWindow(parent) {
    ui();
}

void GraphEditTab::ui() {
    widget = new QWidget;
    setCentralWidget(widget);

    uiHeader();
    uiBody();
    uiFooter();
}

void GraphEditTab::uiHeader() {
    // Mode Bar
    auto* modeBar = addToolBar("Mode Bar");
    auto* modeBtns = new QButtonGroup(this);

    auto* pointerBtn = new QToolButton;
    pointerBtn->setIcon(QIcon(":pointer.svg"));
    pointerBtn->setCheckable(true);
    pointerBtn->setChecked(true);
    modeBtns->addButton(pointerBtn);
    modeBar->addWidget(pointerBtn);

    auto* arrowBtn = new QToolButton;
    arrowBtn->setIcon(QIcon(":arrow.svg"));
    arrowBtn->setCheckable(true);
    modeBtns->addButton(arrowBtn);
    modeBar->addWidget(arrowBtn);

    auto* newNodeBtn = new QToolButton;
    newNodeBtn->setIcon(QIcon(":plus.svg"));
    newNodeBtn->setCheckable(true);
    modeBtns->addButton(newNodeBtn);
    modeBar->addWidget(newNodeBtn);

    // Settings Bar
    auto* settingsBar = addToolBar("Settings Bar");

    auto* depthLabel = new QLabel("Depth:");
    settingsBar->addWidget(depthLabel);

    auto* depthSpin = new QSpinBox;
    depthSpin->setRange(5, 500);
    settingsBar->addWidget(depthSpin);

    settingsBar->addSeparator();

    auto* learnedCheckbox = new QCheckBox(tr("Show Learned"));
    settingsBar->addWidget(learnedCheckbox);
}

void GraphEditTab::uiBody() {
    // Main Layout
    bodyVBox = new QVBoxLayout;
    widget->setLayout(bodyVBox);
    bodyVBox->setSpacing(10);

    // Graph Frame
    auto* graphFrame = new QFrame;
    graphFrame->setFrameStyle(QFrame::StyledPanel);
    graphFrame->setMinimumSize({300, 200});
    bodyVBox->addWidget(graphFrame);

    // Demo
    for (int i = 0; i < 5; ++i) {
        auto d = 50 * (i + 1);
        (new GraphNodeWidget(graphFrame))->move(d, d);
    }
}

void GraphEditTab::uiFooter() {
    // Buttons
    auto* hbox = new QHBoxLayout;
    bodyVBox->addLayout(hbox);

    // Graph name
    auto* nameLabel = new QLabel(tr("No Graph Loaded"));
    hbox->addWidget(nameLabel);

    hbox->addStretch(1);

    // Export Button
    auto* exportBtn = new QPushButton("Export");
    hbox->addWidget(exportBtn);
}
