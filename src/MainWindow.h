#pragma once

#include "GraphTab.h"
#include "GraphEditTab.h"

#include <QMainWindow>
#include <QString>

class MainWindow : public QMainWindow {
    public:
        MainWindow(QWidget* parent = nullptr);

    private:
        void ui();

        GraphTab* graphsTab;
        GraphEditTab* graphEditTab;
};
