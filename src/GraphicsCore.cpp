#include "GraphicsCore.h"
#include "ThemesTab.h"
#include "PackagesTab.h"
#include "LearningGraphTab.h"
#include "LearningListTab.h"

#include <QTabWidget>
#include <QMessageBox>
#include <QSplitter>

GraphicsCore::GraphicsCore(QWidget* parent)
        : QMainWindow(parent) {
    setWindowTitle(tr("Learning Graph"));

    auto* splitter = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(splitter);

    // Left Tabs
    auto* leftTabs = new QTabWidget;
    splitter->addWidget(leftTabs);

    auto* themesTab = new ThemesTab;
    leftTabs->addTab(themesTab, tr("Themes"));

    auto* packagesTab = new PackagesTab;
    leftTabs->addTab(packagesTab, tr("Packages"));

    // Right tabs
    auto* rightTabs = new QTabWidget;
    splitter->addWidget(rightTabs);

    auto* graphTab = new LearningGraphTab;
    rightTabs->addTab(graphTab, tr("Learning Graph"));

    auto* listTab = new LearningListTab;
    rightTabs->addTab(listTab, tr("Learning List"));

    // Splitter stretch
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
}
