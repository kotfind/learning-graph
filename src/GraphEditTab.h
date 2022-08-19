#pragma once

#include <QMainWindow>
#include <QString>
#include <QWidget>
#include <QVBoxLayout>

class GraphEditTab : public QMainWindow {
    public:
        GraphEditTab(QWidget* parent = nullptr);

    private:
        void ui();

        void uiHeader();
        void uiBody();
        void uiFooter();

        int themeId;

        QWidget* widget;
        QVBoxLayout* bodyVBox;
};
