#pragma once

#include "PackageComboBox.h"
#include "SmartListWidget.h"

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QKeyEvent>

class ThemeTab : public QWidget {
    Q_OBJECT

    public:
        ThemeTab(QWidget* parent = nullptr);

    private:
        // Draws user interface
        void ui();

        SmartListWidget* themesList;
        QLineEdit* nameEdit;
        PackageComboBox* packageComboBox;
        QCheckBox* wishlistCheckBox;
        QCheckBox* learnedCheckBox;
        QCheckBox* autoUpdateCheckBox;
        QPushButton* createButton;
        QPushButton* searchButton;
        QCheckBox* selectionModeCheckBox;
        QPushButton* selectAllButton;
        QPushButton* exportButton;

    protected:
        // Emits update on Enter/ Return key pressed
        void keyPressEvent(QKeyEvent*) override;

    public slots:
        // (Re)loads data from db
        void update();

        // Sets autoupdate mode to state
        void setAutoUpdate(bool state);

    private slots:
        // Opens ThemeInfoDialog for package with id packageId
        void onThemeDoubleClicked(int themeId);

        // Opens ThemeContextMenu for theme with id themeId
        void onThemeMenuRequested(int themeId, const QPoint& globalPos);

        // Creates QDrag with current themeId
        void onThemeDragRequested(int themeId);

        // Turns selection mode on if state == true
        // turns it off otherwise
        void onSelectionModeCheckChanged(int state);

        // Selects all if something was selected
        // Clears selection otherwise
        void onSelectAllButtonPressed();

        // Updates selectAll button text
        void onSelectionChanged();

        // Exports theme
        void onExportButtonPressed();
};
