#pragma once

#include <QWidget>
#include <QString>

class ThemeInfoWindow : public QWidget {
    public:
        ThemeInfoWindow(const QString& themeName, QWidget* parent = nullptr);

    private:
        QString themeName;
};
