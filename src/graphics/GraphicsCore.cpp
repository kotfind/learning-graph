#include "GraphicsCore.h"
#include "ThemesTab.h"
#include "PackagesTab.h"

#include <QTabWidget>

GraphicsCore::GraphicsCore(QWidget* parent)
        : QMainWindow(parent) {
    setWindowTitle(tr("Learning Graph"));

    auto* tabs = new QTabWidget;
    setCentralWidget(tabs);

    auto* themesTab = new ThemesTab;
    tabs->addTab(themesTab, tr("Themes"));

    auto* packagesTab = new PackagesTab;
    tabs->addTab(packagesTab, tr("Packages"));
}
