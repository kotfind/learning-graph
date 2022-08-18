#pragma once

#include <QMainWindow>
#include <QString>

class GraphEditTab : public QMainWindow {
    public:
        GraphEditTab(QWidget* parent = nullptr);

    private:
        void ui();

        int themeId;
};
