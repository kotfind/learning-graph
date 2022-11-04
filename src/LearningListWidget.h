#pragma once

#include "SmartListWidget.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class LearningListWidget : public QWidget {
    Q_OBJECT

    public:
        LearningListWidget(QWidget* parent = nullptr);

    private:
        void ui();

        QLabel* mainLabel;
        SmartListWidget* themesList;
        QPushButton* exportButton;
        QPushButton* closeButton;

    private slots:
        // Inits build of learning list for theme themeId
        //    and opens it
        void open(int themeId);

        // Closes opened learning list
        void close();

        // Opens ThemeContextMenu for theme with id themeId
        void onThemeMenuRequested(int themeId, const QPoint& globalPos);

        // Exports Learning List
        void onExportButtonClicked();
};
