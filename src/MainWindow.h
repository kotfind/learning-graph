#pragma once

#include "GraphTab.h"
#include "GraphEditWidget.h"

#include <QMainWindow>
#include <QString>
#include <QActionGroup>

class MainWindow : public QMainWindow {
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
        GraphEditWidget* graphEditTab;

        QActionGroup* fontSizeActionGroup;

    private slots:
        // Sets font
        void onFontSizeActionTriggered(QAction*);
};
