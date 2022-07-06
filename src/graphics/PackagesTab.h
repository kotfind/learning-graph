#pragma once

#include "../datatypes/PackageRequest.h"
#include "../datatypes/Package.h"

#include <QWidget>
#include <QListWidget>

class PackagesTab : public QWidget {
    Q_OBJECT

    public:
        PackagesTab(QWidget* parent = nullptr);

    private:
        QListWidget* packagesList;

    signals:
        void listRequested(const PackageRequest& fields);
        void creationRequested(const QString& package);

    private slots:
        void onCreateBtn();
        void onListGot(const QVector<Package>& packages);
        void onPackagesChanged();
};
