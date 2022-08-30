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
#include <QSettings>
#include <QStatusBar>

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

    connect(
        scaleSpinBox,
        &ScaleSpinBox::valueChanged,
        graphView,
        &GraphView::setScale
    );

    connect(
        graphView,
        &GraphView::scaleChanged,
        scaleSpinBox,
        &QDoubleSpinBox::setValue
    );

    connect(
        graphScene,
        &GraphScene::underCursorItemChanged,
        this,
        &GraphEditTab::setunderCursorItem
    );

    connect(
        graphScene,
        &GraphScene::underCursorItemChanged,
        graphView,
        &GraphView::setunderCursorItem
    );

    emit modeChanged(CURSOR_EDIT_MODE);

    // Load from settings
    QSettings settings;
    if (settings.contains("graph/id")) {
        open(settings.value("graph/id").toInt());
    }
}

void GraphEditTab::ui() {
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

    auto* learnedCheckbox = new QCheckBox(tr("Show Learned"));
    settingsBar->addWidget(learnedCheckbox);

    settingsBar->addSeparator();

    settingsBar->addWidget(new QLabel(tr("Scale: ")));

    scaleSpinBox = new ScaleSpinBox;
    settingsBar->addWidget(scaleSpinBox);
}

void GraphEditTab::uiBody() {
    // Graph Frame
    graphView = new GraphView;
    graphView->setMinimumSize({300, 200});
    setCentralWidget(graphView);
}

void GraphEditTab::uiFooter() {
    nameLabel = new QLabel(tr("No Graph Loaded"));

    statusBar()->addWidget(nameLabel);
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

    nameLabel->setText(tr("[Graph] %1").arg(query.value(0).toString()));
    graphScene->open(graphId);
    graphView->setDisabled(false);

    // Write to settings
    QSettings settings;
    settings.setValue("graph/id", graphId);
}

void GraphEditTab::setunderCursorItem(QGraphicsItem* item) {
    GraphNode* node;
    GraphEdge* edge;
    if (node = qgraphicsitem_cast<GraphNode*>(item)) {
        statusBar()->showMessage(tr("[Node] %1").arg(node->toPlainText()));
    } else if (edge = qgraphicsitem_cast<GraphEdge*>(item)) {
        statusBar()->showMessage(tr("[Edge]"));
    } else {
        statusBar()->clearMessage();
    }
}
