#include "GraphEditTab.h"

#include <QWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QIcon>
#include <QSlider>
#include <QLabel>

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
}

void GraphEditTab::uiBody() {
    // Main Layout
    bodyVBox = new QVBoxLayout;
    widget->setLayout(bodyVBox);
    bodyVBox->setSpacing(10);

    // Place for Graph
    auto* frame = new QFrame;
    frame->setFrameStyle(QFrame::StyledPanel);
    frame->setMinimumSize({300, 200});
    bodyVBox->addWidget(frame);
}

void GraphEditTab::uiFooter() {
    // Buttons
    auto* hbox = new QHBoxLayout;
    bodyVBox->addLayout(hbox);

    hbox->addStretch(1);

    // Depth Slider
    auto* depthLabel = new QLabel("Depth:");
    hbox->addWidget(depthLabel);

    auto* depthSlider = new QSlider(Qt::Horizontal);
    depthSlider->setMinimumWidth(200);
    depthSlider->setRange(5, 100);
    hbox->addWidget(depthSlider);

    auto* depthNumLabel = new QLabel;
    connect(
        depthSlider,
        &QSlider::valueChanged,
        [depthNumLabel](int val) {
            depthNumLabel->setText(QString("(%1)").arg(val, 3));
        }
    );
    hbox->addWidget(depthNumLabel);
    depthSlider->setValue(5);
    emit depthSlider->valueChanged(5); // idkn last line won't do it

    // Learned Checkbox
    auto* learnedCheckbox = new QCheckBox(tr("Show Learned Themes"));
    hbox->addWidget(learnedCheckbox);

    // Export Button
    auto* exportBtn = new QPushButton("Export");
    hbox->addWidget(exportBtn);
}
