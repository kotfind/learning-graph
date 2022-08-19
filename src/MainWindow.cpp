#include "MainWindow.h"

#include "ThemeTab.h"
#include "PackageTab.h"
#include "GraphTab.h"
#include "GraphEditTab.h"
#include "LearningListTab.h"

#include <QTabWidget>
#include <QMessageBox>
#include <QSplitter>

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent) {
    setWindowTitle(tr("Learning Graph"));
    ui();
}

void MainWindow::ui() {
    auto* splitter = new QSplitter(Qt::Horizontal);
    setCentralWidget(splitter);

    // Left Tabs
    auto* leftTabs = new QTabWidget;
    splitter->addWidget(leftTabs);

    auto* themesTab = new ThemeTab;
    leftTabs->addTab(themesTab, tr("Themes"));

    auto* packagesTab = new PackageTab;
    leftTabs->addTab(packagesTab, tr("Packages"));

    auto* graphsTab = new GraphTab;
    leftTabs->addTab(graphsTab, tr("Graphs"));

    // Right tabs
    auto* rightTabs = new QTabWidget;
    splitter->addWidget(rightTabs);

    auto* graphTab = new GraphEditTab;
    rightTabs->addTab(graphTab, tr("Learning Graph"));

    auto* listTab = new LearningListTab;
    rightTabs->addTab(listTab, tr("Learning List"));

    // Splitter stretch
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
}
