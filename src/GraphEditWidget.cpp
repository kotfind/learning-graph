#include "GraphEditWidget.h"

#include "GraphNodeItem.h"
#include "db/db.h"
#include "appendExtention.h"
#include "GlobalSignalHandler.h"
#include "filesystem/filesystem.h"

#include <QWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QIcon>
#include <QSettings>
#include <QStatusBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSvgGenerator>
#include <QPainter>

GraphEditWidget::GraphEditWidget(QWidget* parent)
        : QMainWindow(parent) {
    ui();

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

    connect(
        exportButton,
        &QPushButton::clicked,
        this,
        &GraphEditWidget::exportGraph
    );

    connect(
        closeButton,
        &QPushButton::clicked,
        this,
        &GraphEditWidget::close
    );

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::graphsUpdated,
        this,
        &GraphEditWidget::onGraphsUpdated
    );

    connect(
        clearDeletedButton,
        &QPushButton::clicked,
        this,
        &GraphEditWidget::onClearDeleteButtonClicked
    );

    graphScene->setMode(CURSOR_EDIT_MODE);

    setDisabled(true);

    // Load from settings
    QSettings settings;
    if (settings.contains("graph/id")) {
        open(settings.value("graph/id").toInt()); // XXX Add existance check ?
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
    modeBar = addToolBar(tr("Mode Bar"));

    auto* modeButtons = new QButtonGroup(this);
    connect(
        modeButtons,
        &QButtonGroup::idPressed,
        [this] (int id) {
            graphScene->setMode((GraphEditMode)id);
        }
    );

    auto* pointerButton = new QToolButton;
    pointerButton->setIcon(QIcon(":pointer.svg"));
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    modeButtons->addButton(pointerButton, CURSOR_EDIT_MODE);
    modeBar->addWidget(pointerButton);

    auto* arrowButton = new QToolButton;
    arrowButton->setIcon(QIcon(":arrow.svg"));
    arrowButton->setCheckable(true);
    modeButtons->addButton(arrowButton, EDGE_EDIT_MODE);
    modeBar->addWidget(arrowButton);

    auto* newNodeButton = new QToolButton;
    newNodeButton->setIcon(QIcon(":plus.svg"));
    newNodeButton->setCheckable(true);
    modeButtons->addButton(newNodeButton, NEW_NODE_EDIT_MODE);
    modeBar->addWidget(newNodeButton);

    auto* deleteButton = new QToolButton;
    deleteButton->setIcon(QIcon(":cross.svg"));
    deleteButton->setCheckable(true);
    modeButtons->addButton(deleteButton, DELETE_EDIT_MODE);
    modeBar->addWidget(deleteButton);

    // Scale Bar
    scaleBar = addToolBar(tr("Scale Bar"));

    scaleBar->addWidget(new QLabel(tr("Scale: ")));

    scaleSpinBox = new ScaleSpinBox;
    scaleBar->addWidget(scaleSpinBox);

    // Other Buttons Bar
    otherButtonsBar = addToolBar(tr("Other Buttons Bar"));

    exportButton = new QPushButton(tr("Export"));
    otherButtonsBar->addWidget(exportButton);

    otherButtonsBar->addSeparator();

    closeButton = new QPushButton(tr("Close"));
    otherButtonsBar->addWidget(closeButton);

    otherButtonsBar->addSeparator();

    clearDeletedButton = new QPushButton(tr("Clear Deleted Themes"));
    otherButtonsBar->addWidget(clearDeletedButton);
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

    nameLabel->setText(tr("[Graph] %1").arg(db::graph::name(graphId)));
    graphScene->open(graphId);

    setDisabled(false);

    // Write to settings
    QSettings settings;
    settings.setValue("graph/id", graphId);
}

void GraphEditWidget::close() {
    this->graphId = -1;

    nameLabel->setText(tr("No Graph Loaded"));
    graphScene->close();

    setDisabled(true);

    scaleSpinBox->setValue(1);

    // Write to settings
    QSettings settings;
    settings.remove("graph/id");
}

void GraphEditWidget::updateStatus(QMouseEvent* e) {
    auto* item = graphScene->itemAt(
        graphView->mapToScene(
            graphView->mapFromParent(e->pos())
        ),
        QTransform()
    );

    GraphNodeItem* node;
    GraphEdge* edge;
    if (node = qgraphicsitem_cast<GraphNodeItem*>(item)) {
        statusBar()->showMessage(tr("[Theme] %1").arg(node->toPlainText()));
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

void GraphEditWidget::exportGraph() {
    const QString jpgFilter = tr("JPG image (*.jpg)");
    const QString pngFilter = tr("PNG image (*.png)");
    const QString svgFilter = tr("SVG image (*.svg)");
    const QString graphFilter = tr("Learning Graph graph (*.graph)");

    QString selectedFilter;
    auto filename = QFileDialog::getSaveFileName(
        this,
        tr("Export to ..."),
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
        jpgFilter + ";;" + pngFilter + ";;" + svgFilter + ";;" + graphFilter,
        &selectedFilter
    );

    if (filename.isEmpty()) {
        return;
    }

    if (selectedFilter == jpgFilter) {
        appendExtentionIfNot(filename, ".jpg");
        filesystem::graph::exportAsJpg(filename, graphScene);
    } else if (selectedFilter == pngFilter) {
        appendExtentionIfNot(filename, ".png");
        filesystem::graph::exportAsPng(filename, graphScene);
    } else if (selectedFilter == svgFilter) {
        appendExtentionIfNot(filename, ".svg");
        filesystem::graph::exportAsSvg(filename, graphScene);
    } else {
        appendExtentionIfNot(filename, ".graph");
        filesystem::graph::exportAsGraph(filename, graphId);
    }
}

void GraphEditWidget::onGraphsUpdated() {
    if (graphId != -1 && !db::graph::exists(graphId)) {
        close();
    }
}

void GraphEditWidget::onClearDeleteButtonClicked() {
    db::graphNode::deleteDeletedThemes(graphId);
    open(graphId);
}
