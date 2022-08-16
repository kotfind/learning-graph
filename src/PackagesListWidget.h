#pragma once

#include <QListWidget>
#include <QVector>
#include <QPoint>

class PackagesListWidget : public QListWidget {
    Q_OBJECT

    public:
        PackagesListWidget(QWidget* parent = nullptr);

    private slots:
        void showContextMenu(const QPoint& pos);
};
