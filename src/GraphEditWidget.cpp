#include "GraphEditWidget.h"

#include "GraphNode.h"
#include "db/sqlDefines.h"

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

GraphEditWidget::GraphEditWidget(QWidget* parent)
        : QMainWindow(parent) {
    ui();

    graphView->setDisabled(true);

    graphScene = new GraphScene;
    graphView->setScene(graphScene);

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

    graphScene->setMode(CURSOR_EDIT_MODE);

    // Load from settings
    QSettings settings;
    if (settings.contains("graph/id")) {
        open(settings.value("graph/id").toInt());
    }

    setMouseTracking(true);
}

void GraphEditWidget::ui() {
    uiHeader();
    uiBody();
    uiFooter();
}

void GraphEditWidget::uiHeader() {
    // Mode Bar
    auto* modeBar = addToolBar("Mode Bar");
    modeBar->setContextMenuPolicy(Qt::PreventContextMenu);


    auto* modeBtns = new QButtonGroup(this);
    connect(
        modeBtns,
        &QButtonGroup::idPressed,
        [this] (int id) {
            graphScene->setMode((GraphEditMode)id);
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

void GraphEditWidget::uiBody() {
    // Graph Frame
    graphView = new GraphView;
    graphView->setMinimumSize({300, 200});
    setCentralWidget(graphView);
}

void GraphEditWidget::uiFooter() {
    nameLabel = new QLabel(tr("No Graph Loaded"));

    statusBar()->addWidget(nameLabel);
}

void GraphEditWidget::open(int graphId) {
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

void GraphEditWidget::updateStatus(QMouseEvent* e) {
    auto* item = graphScene->itemAt(
        graphView->mapToScene(
            graphView->mapFromParent(e->pos())
        ),
        QTransform()
    );

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

void GraphEditWidget::mouseMoveEvent(QMouseEvent* e) {
    updateStatus(e);
    QMainWindow::mouseMoveEvent(e);
}

void GraphEditWidget::mousePressEvent(QMouseEvent* e) {
    updateStatus(e);
    QMainWindow::mousePressEvent(e);
}

void GraphEditWidget::mouseReleaseEvent(QMouseEvent* e) {
    updateStatus(e);
    QMainWindow::mouseReleaseEvent(e);
}
