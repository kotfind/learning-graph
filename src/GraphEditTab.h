#pragma once

#include <QWidget>
#include <QString>

class GraphEditTab : public QWidget {
    public:
        GraphEditTab(QWidget* parent = nullptr);

    private:
        void ui();

        int themeId;
};
