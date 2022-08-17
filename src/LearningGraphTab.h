#pragma once

#include <QWidget>
#include <QString>

class LearningGraphTab : public QWidget {
    public:
        LearningGraphTab(QWidget* parent = nullptr);

    private:
        void ui();

        int themeId;
};
