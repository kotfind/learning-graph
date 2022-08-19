#include "GraphEditTab.h"

#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>
#include <QButtonGroup>
#include <QIcon>

GraphEditTab::GraphEditTab(QWidget* parent)
        : QMainWindow(parent) {
    ui();
}

void GraphEditTab::ui() {
    auto* widget = new QWidget;
    setCentralWidget(widget);

    // Toolbar
    auto* toolbar = addToolBar("Graph Toolbar");
    auto* toolBtns = new QButtonGroup(this);

    auto* pointerBtn = new QToolButton;
    pointerBtn->setIcon(QIcon(":pointer.svg"));
    pointerBtn->setCheckable(true);
    pointerBtn->setChecked(true);
    toolBtns->addButton(pointerBtn);
    toolbar->addWidget(pointerBtn);

    auto* arrowBtn = new QToolButton;
    arrowBtn->setIcon(QIcon(":arrow.svg"));
    arrowBtn->setCheckable(true);
    toolBtns->addButton(arrowBtn);
    toolbar->addWidget(arrowBtn);

    auto* newNodeBtn = new QToolButton;
    newNodeBtn->setIcon(QIcon(":plus1.svg"));
    newNodeBtn->setCheckable(true);
    toolBtns->addButton(newNodeBtn);
    toolbar->addWidget(newNodeBtn);

    auto* addNodeBtn = new QToolButton;
    addNodeBtn->setIcon(QIcon(":plus2.svg"));
    addNodeBtn->setCheckable(true);
    toolBtns->addButton(addNodeBtn);
    toolbar->addWidget(addNodeBtn);

    // Main Layout
    auto* vbox = new QVBoxLayout;
    widget->setLayout(vbox);
    vbox->setSpacing(10);

    // Place for Graph
    auto* frame = new QFrame;
    frame->setFrameStyle(QFrame::StyledPanel);
    frame->setMinimumSize({300, 200});
    vbox->addWidget(frame);

    // Buttons
    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    hbox->addStretch(1);

    auto* exportBtn = new QPushButton("Export");
    hbox->addWidget(exportBtn);
}
