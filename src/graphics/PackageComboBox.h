#pragma once

#include "../datatypes/Package.h"
#include "../datatypes/PackageRequest.h"

#include <QComboBox>
#include <QVector>

class PackageComboBox : public QComboBox {
    Q_OBJECT

    public:
        PackageComboBox(QWidget* parent = nullptr);
        void setCurrent(int packageId);

    private:
        int currentId;

    signals:
        void listRequested(const PackageRequest& fields);

    private slots:
        void onListGot(const QVector<Package>& packages);
};
