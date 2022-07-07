#pragma once

#include "../datatypes/Package.h"

#include <QListWidget>
#include <QVector>
#include <QPoint>

class PackagesListWidget : public QListWidget {
    Q_OBJECT

    public:
        PackagesListWidget(QWidget* parent = nullptr);

    signals:
        void getList();
        void deletePackage(int packageId);
        void updatePackage(const Package& package);

    private slots:
        void onListGot(const QVector<Package>& packages);
        void showContextMenu(const QPoint& pos);
};
