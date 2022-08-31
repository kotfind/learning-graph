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
        void ui();
        void uiHeader();
        void uiBody();

        GraphTab* graphsTab;
        GraphEditWidget* graphEditTab;

        QActionGroup* fontSizeActionGroup;

    private slots:
        void onFontSizeActionTriggered(QAction*);
};
