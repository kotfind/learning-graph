#pragma once

#include <QWidget>
#include <QString>

class LearningGraphWindow : public QWidget {
    public:
        LearningGraphWindow(int themeId, QWidget* parent = nullptr);

    private:
        int themeId;
};
