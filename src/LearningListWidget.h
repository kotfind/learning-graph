#pragma once

#include "SmartListWidget.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

class LearningListWidget : public QWidget {
    Q_OBJECT

    public:
        LearningListWidget(QWidget* parent = nullptr);

    private:
        // Draws user interface
        void ui();

        // (Re)loads data from db
        void load();

        QLabel* mainLabel;
        QCheckBox* showLearnedCheckBox;
        SmartListWidget* themesList;
        QPushButton* exportButton;
        QPushButton* closeButton;

    private slots:
        // Inits build of learning list for theme themeId
        //    and opens it
        void open(int themeId);

        // Closes opened learning list
        void close();

        // Creates QDrag with current themeId
        void onThemeDragRequested(int themeId);

        // Opens ThemeContextMenu for theme with id themeId
        void onThemeMenuRequested(int themeId, const QPoint& globalPos);

        // Exports Learning List
        void onExportButtonClicked();

        // Checks if some of list's themes were deleted
        void onThemesUpdated();

        // Shows or hides learned themes from list
        void onShowLearnedCheckBoxChanged(int state);
};
