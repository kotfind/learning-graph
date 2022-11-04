#pragma once

#include <QMenu>

class ThemeContextMenu : public QMenu {
    Q_OBJECT

    public:
        ThemeContextMenu(int themeId, QWidget* parent = nullptr);

    private:
        // Draws user interface
        void ui();

        int themeId;

    signals:
        void themesUpdated();
        void openList(int themeId);
};
