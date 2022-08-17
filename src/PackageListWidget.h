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
        void themesUpdated();

    public slots:
        void update();

    private slots:
        void showContextMenu(const QPoint& pos);
};
