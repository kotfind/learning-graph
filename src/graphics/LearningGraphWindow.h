#pragma once

#include <QWidget>
#include <QString>

class LearningGraphWindow : public QWidget {
    public:
        LearningGraphWindow(const QString& themeName, QWidget* parent = nullptr);

    private:
        QString themeName;
};
