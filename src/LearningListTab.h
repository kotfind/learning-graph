#pragma once

#include <QWidget>
#include <QString>

class LearningListTab : public QWidget {
    public:
        LearningListTab(QWidget* parent = nullptr);

    private:
        void ui();

        int themeId;
};
