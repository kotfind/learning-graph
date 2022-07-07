#pragma once

#include "../datatypes/Package.h"

#include <QComboBox>
#include <QVector>

class PackageComboBox : public QComboBox {
    Q_OBJECT

    public:
        PackageComboBox(QWidget* parent = nullptr);
        void setCurrent(int packageId);
        void setAny(bool v);

    private:
        int currentId;
        bool hasOptionAny = false;

    signals:
        void getList();

    private slots:
        void onListGot(const QVector<Package>& packages);
};
