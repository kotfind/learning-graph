#pragma once

#include <QListWidget>
#include <QVector>
#include <QPoint>

class PackageListWidget : public QListWidget {
    Q_OBJECT

    public:
        PackageListWidget(QWidget* parent = nullptr);

    private slots:
        void showContextMenu(const QPoint& pos);
};
