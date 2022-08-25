#pragma once

#include "GraphTab.h"
#include "GraphEditTab.h"

#include <QMainWindow>
#include <QString>
#include <QActionGroup>

class MainWindow : public QMainWindow {
    public:
        MainWindow(QWidget* parent = nullptr);

    private:
        void ui();
        void uiHeader();
        void uiBody();

        GraphTab* graphsTab;
        GraphEditTab* graphEditTab;

        QActionGroup* fontSizeActionGroup;

    private slots:
        void onFontSizeActionTriggered(QAction*);
};
