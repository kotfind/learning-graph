#include "GraphEditTab.h"

#include "GraphNode.h"
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
#include <QSettings>

GraphEditTab::GraphEditTab(QWidget* parent)
        : QMainWindow(parent) {
    ui();

    graphView->setDisabled(true);

    graphScene = new GraphScene;
    graphView->setScene(graphScene);

    connect(
        this,
        &GraphEditTab::modeChanged,
        graphScene,
        &GraphScene::setMode
    );

    connect(
        this,
        &GraphEditTab::modeChanged,
        graphView,
        &GraphView::setMode
    );

    emit modeChanged(CURSOR_EDIT_MODE);

    // Load from settings
    QSettings settings;
    if (settings.contains("graph/id")) {
        open(settings.value("graph/id").toInt());
    }
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
    modeBar->setContextMenuPolicy(Qt::PreventContextMenu);


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
    modeBtns->addButton(arrowBtn, EDGE_EDIT_MODE);
    modeBar->addWidget(arrowBtn);

    auto* newNodeBtn = new QToolButton;
    newNodeBtn->setIcon(QIcon(":plus.svg"));
    newNodeBtn->setCheckable(true);
    modeBtns->addButton(newNodeBtn, NEW_NODE_EDIT_MODE);
    modeBar->addWidget(newNodeBtn);

    auto* deleteBtn = new QToolButton;
    deleteBtn->setIcon(QIcon(":cross.svg"));
    deleteBtn->setCheckable(true);
    modeBtns->addButton(deleteBtn, DELETE_EDIT_MODE);
    modeBar->addWidget(deleteBtn);

    // Settings Bar
    auto* settingsBar = addToolBar("Settings Bar");
    settingsBar->setContextMenuPolicy(Qt::PreventContextMenu);

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
    graphView = new GraphView;
    graphView->setMinimumSize({300, 200});
    bodyVBox->addWidget(graphView);
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

    PREPARE_NEW(query, " \
        SELECT name \
        FROM graphs \
        WHERE id = ? \
    ")
    query.addBindValue(graphId);
    EXEC(query)
    query.next();

    nameLabel->setText(query.value(0).toString());
    graphScene->open(graphId);
    graphView->setDisabled(false);

    // Write to settings
    QSettings settings;
    settings.setValue("graph/id", graphId);
}
