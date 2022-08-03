#include "GraphicsCore.h"
#include "ThemesTab.h"
#include "PackagesTab.h"

#include "../logics/WorkerCore.h"

#include <QTabWidget>
#include <QMessageBox>

GraphicsCore::GraphicsCore(QWidget* parent)
        : QMainWindow(parent) {
    setWindowTitle(tr("Learning Graph"));

    auto* tabs = new QTabWidget;
    setCentralWidget(tabs);

    auto* themesTab = new ThemesTab;
    tabs->addTab(themesTab, tr("Themes"));

    auto* packagesTab = new PackagesTab;
    tabs->addTab(packagesTab, tr("Packages"));

    // Connections
    connect(WorkerCore::getInstance(), &WorkerCore::errorGot,
            this, &GraphicsCore::onErrorGot);
}

void GraphicsCore::onErrorGot(const QString& error) {
    QMessageBox::critical(this, tr("Error"), error);
}
