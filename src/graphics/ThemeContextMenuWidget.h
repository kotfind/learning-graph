#pragma once

#include <QMenu>
#include <QString>

class ThemeContextMenuWidget : public QMenu {
    Q_OBJECT

    public:
        ThemeContextMenuWidget(int themeId, const QString& themeName, QWidget* parent = nullptr);

    signals:
        void deleteTheme(int themeId);
};
