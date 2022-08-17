#pragma once

#include <QMenu>
#include <QString>

class ThemeContextMenu : public QMenu {
    Q_OBJECT

    public:
        ThemeContextMenu(int themeId, const QString& themeName, QWidget* parent = nullptr);

    signals:
        void themesUpdated();
};
