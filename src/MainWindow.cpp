#include "MainWindow.h"

#include "LearningListWidget.h"
#include "ThemeTab.h"
#include "PackageTab.h"
#include "GlobalSignalHandler.h"

#include <QTabWidget>
#include <QMessageBox>
#include <QSplitter>
#include <QMenuBar>
#include <QMenu>
#include <QApplication>
#include <QFontDatabase>
#include <QSettings>

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent) {
    setWindowTitle(tr("Learning Graph"));

    // Load settings
    QSettings settings;
    auto font = qvariant_cast<QFont>(settings.value("font", qApp->font()));
    qApp->setFont(font);
    emit GlobalSignalHandler::getInstance()->fontSet(font);

    ui();

    connect(
        graphsTab,
        &GraphTab::open,
        graphEditWidget,
        &GraphEditWidget::open
    );
}

void MainWindow::ui() {
    uiHeader();
    uiBody();
}

void MainWindow::uiHeader() {
    auto* settingsMenu = menuBar()->addMenu("Settings");

    // Fonts
    auto* fontSizeMenu = settingsMenu->addMenu("Font Size");

    fontSizeActionGroup = new QActionGroup(this);
    fontSizeActionGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);

    auto font = qApp->font();
    qApp->setFont(font);
    emit GlobalSignalHandler::getInstance()->fontSet(font);

    connect(
        fontSizeActionGroup,
        &QActionGroup::triggered,
        this,
        &MainWindow::onFontSizeActionTriggered
    );

    for (int size : QFontDatabase::pointSizes(qApp->font().family())) {
        auto* action = new QAction(QString::number(size), this);

        action->setCheckable(true);
        action->setChecked(size == qApp->font().pointSize());
        action->setData(size);

        // NOTE: idk why it won't work:
        // auto font = qApp->font();
        // font.setPointSize(size);
        // action->setFont(font);

        fontSizeActionGroup->addAction(action);
        fontSizeMenu->addAction(action);
    }
}

void MainWindow::uiBody() {
    auto* splitter = new QSplitter(Qt::Horizontal);
    setCentralWidget(splitter);

    // Left Tabs
    auto* leftTabs = new QTabWidget;
    splitter->addWidget(leftTabs);

    auto* themesTab = new ThemeTab;
    leftTabs->addTab(themesTab, tr("Themes"));

    auto* packagesTab = new PackageTab;
    leftTabs->addTab(packagesTab, tr("Packages"));

    graphsTab = new GraphTab;
    leftTabs->addTab(graphsTab, tr("Graphs"));

    // Graph Edit tab
    graphEditWidget = new GraphEditWidget;
    splitter->addWidget(graphEditWidget);

    // Learning List tab
    learningListWidget = new LearningListWidget;
    splitter->addWidget(learningListWidget);

    // Splitter stretch
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
}

void MainWindow::onFontSizeActionTriggered(QAction* action) {
    auto font = qApp->font();
    font.setPointSize(action->data().toInt());
    qApp->setFont(font);
    emit GlobalSignalHandler::getInstance()->fontSet(font);

    QSettings settings;
    settings.setValue("font", font);
}
