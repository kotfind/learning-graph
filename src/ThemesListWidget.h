#pragma once

#include <QListWidget>
#include <QVector>

class ThemesListWidget : public QListWidget {
    Q_OBJECT

    public:
        ThemesListWidget(QWidget* parent = nullptr);

    private slots:
        void showContextMenu(const QPoint& pos);
};
