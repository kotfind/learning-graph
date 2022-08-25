#pragma once

#include <QMenu>

class ThemeContextMenu : public QMenu {
    Q_OBJECT

    public:
        ThemeContextMenu(int themeId, QWidget* parent = nullptr);

    private:
        void ui();

        int themeId;

    signals:
        void themesUpdated();
};
