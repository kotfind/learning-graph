#pragma once

#include <QMenu>
#include <QString>

class ThemeContextMenuWidget : public QMenu {
    Q_OBJECT

    public:
        ThemeContextMenuWidget(int themeId, QWidget* parent = nullptr);

    private:
        int themeId;
};
