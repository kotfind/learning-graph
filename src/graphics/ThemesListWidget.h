#pragma once

#include "../logics/Theme.h"

#include <QListWidget>
#include <QVector>

class ThemesListWidget : public QListWidget {
    Q_OBJECT

    public:
        ThemesListWidget(QWidget* parent = nullptr);

    signals:

    private slots:
        void onListGot(const QVector<Theme>& themes);
        void showContextMenu(const QPoint& pos);
};
