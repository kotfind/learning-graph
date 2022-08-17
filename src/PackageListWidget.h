#pragma once

#include <QListWidget>
#include <QVector>
#include <QPoint>

class PackageListWidget : public QListWidget {
    Q_OBJECT

    public:
        PackageListWidget(QWidget* parent = nullptr);

    signals:
        void packagesUpdated();

    public slots:
        void update();

    private slots:
        void showContextMenu(const QPoint& pos);
};
