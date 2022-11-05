#pragma once

#include "SmartListWidget.h"

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QMenu>
#include <QAction>

class PackageTab : public QWidget {
    Q_OBJECT

    public:
        PackageTab(QWidget* parent = nullptr);

    private:
        // Draws user interface
        void ui();

        QPushButton* createButton;
        QPushButton* importButton;
        QLineEdit* nameEdit;
        QCheckBox* autoUpdateCheckBox;
        QPushButton* updateButton;
        SmartListWidget* packagesList;
        QPushButton* selectAllButton;
        QPushButton* exportButton;

        QMenu* createMenu;
        QAction* createEmptyPackageAction;
        QAction* generatePackageAction;

    signals:
        void packagesUpdated();
        void themesUpdated();

    public slots:
        // (Re)loads data from db
        void update();

        // Sets autoupdate mode to state
        void setAutoUpdate(bool state);

    private slots:
        // Opens PackageInfoDialog for package with id packageId
        void onPackageDoubleClicked(int packageId);

        // Opens context menu for package with id packageId
        void onPackageMenuRequested(int packageId, const QPoint& globalPos);

        // Selects all if something was selected
        // Clears selection otherwise
        void onSelectAllButtonClicked();

        // Updates selectAll button text
        void onSelectionChanged();

        // Exports selected packages
        void onExportButtonClicked();

        // Imports packages
        void onImportButtonClicked();

        // Creates empty package
        void onCreateEmptyPackageActionTriggered();

        // Generates package
        void onGeneratePackageActionTriggered();
};
