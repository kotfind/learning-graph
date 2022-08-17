#pragma once

#include <QListWidget>
#include <QVector>

class ThemeListWidget : public QListWidget {
    Q_OBJECT

    public:
        ThemeListWidget(QWidget* parent = nullptr);

    private slots:
        void showContextMenu(const QPoint& pos);
};
