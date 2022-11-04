#pragma once

#include "GraphTab.h"
#include "GraphEditWidget.h"
#include "LearningListWidget.h"

#include <QMainWindow>
#include <QString>
#include <QActionGroup>

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);

    private:
        // Draws user interface
        void ui();

        // Draws user interface (header)
        void uiHeader();

        // Draws user interface (body)
        void uiBody();

        GraphTab* graphsTab;
        GraphEditWidget* graphEditWidget;
        LearningListWidget* learningListWidget;
        QActionGroup* fontSizeActionGroup;
        QActionGroup* languageActionGroup;

    private slots:
        // Sets font
        void onFontSizeActionTriggered(QAction*);

        // Sets language
        void onLanguageActionTriggered(QAction*);
};
