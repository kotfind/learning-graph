#include "GraphEditTab.h"

#include "GraphNodeWidget.h"
#include "sqlDefines.h"

#include <QWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QIcon>
#include <QSpinBox>

GraphEditTab::GraphEditTab(QWidget* parent)
        : QMainWindow(parent) {
    ui();
    setDisabled(true);

    connect(
        this,
        &GraphEditTab::modeChanged,
        graphCanvas,
        &GraphCanvasWidget::setMode
    );
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
    connect(
        modeBtns,
        &QButtonGroup::idPressed,
        [this] (int id) {
            emit modeChanged((GraphEditMode)id);
        }
    );

    auto* pointerBtn = new QToolButton;
    pointerBtn->setIcon(QIcon(":pointer.svg"));
    pointerBtn->setCheckable(true);
    pointerBtn->setChecked(true);
    modeBtns->addButton(pointerBtn, CURSOR_EDIT_MODE);
    modeBar->addWidget(pointerBtn);

    auto* arrowBtn = new QToolButton;
    arrowBtn->setIcon(QIcon(":arrow.svg"));
    arrowBtn->setCheckable(true);
    modeBtns->addButton(arrowBtn, ARROW_EDIT_MODE);
    modeBar->addWidget(arrowBtn);

    auto* newNodeBtn = new QToolButton;
    newNodeBtn->setIcon(QIcon(":plus.svg"));
    newNodeBtn->setCheckable(true);
    modeBtns->addButton(newNodeBtn, NEW_NODE_EDIT_MODE);
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
    graphCanvas = new GraphCanvasWidget;
    graphCanvas->setFrameStyle(QFrame::StyledPanel);
    graphCanvas->setMinimumSize({300, 200});
    bodyVBox->addWidget(graphCanvas);
}

void GraphEditTab::uiFooter() {
    // Buttons
    auto* hbox = new QHBoxLayout;
    bodyVBox->addLayout(hbox);

    // Graph name
    nameLabel = new QLabel(tr("No Graph Loaded"));
    hbox->addWidget(nameLabel);

    hbox->addStretch(1);

    // Export Button
    auto* exportBtn = new QPushButton("Export");
    hbox->addWidget(exportBtn);
}

void GraphEditTab::open(int graphId) {
    this->graphId = graphId;

    QSqlQuery query;
    LOG_PREPARE(query, " \
        SELECT name \
        FROM graphs \
        WHERE id = ? \
    ")
    query.addBindValue(graphId);
    LOG_EXEC(query)
    query.next();

    nameLabel->setText(query.value(0).toString());
    graphCanvas->open(graphId);
    setDisabled(false);
}
