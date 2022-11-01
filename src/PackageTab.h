#pragma once

#include "SmartListWidget.h"

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

class PackageTab : public QWidget {
    Q_OBJECT

    public:
        PackageTab(QWidget* parent = nullptr);

    private:
        // Draws user interface
        void ui();

        QLineEdit* nameEdit;
        QCheckBox* autoUpdateCheckBox;
        QPushButton* updateButton;
        SmartListWidget* packagesList;
        QPushButton* selectAllButton;
        QPushButton* exportButton;

    signals:
        void packagesUpdated();
        void themesUpdated();

    public slots:
        // (Re)loads data from db
        void update();

        // Sets autoupdate mode to state
        void setAutoUpdate(bool state);

    private slots:
        // Opens creation dialog
        void onCreateBtn();

        // Opens PackageInfoDialog for package with id packageId
        void onPackageDoubleClicked(int packageId);

        // Opens context menu for package with id packageId
        void onPackageMenuRequested(int packageId, const QPoint& globalPos);

        // Selects all if something was selected
        // Clears selection otherwise
        void onSelectAllButtonPressed();

        // Updates selectAll button text
        void onSelectionChanged();

        // Exports package
        void onExportButtonPressed();
};
