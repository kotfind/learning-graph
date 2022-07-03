#pragma once

#include <QWidget>
#include <QString>

class LearningListWindow : public QWidget {
    public:
        LearningListWindow(int themeId, QWidget* parent = nullptr);

    private:
        int themeId;
};
