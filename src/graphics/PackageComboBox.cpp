#include "PackageComboBox.h"
#include "../logics/WorkerCore.h"

PackageComboBox::PackageComboBox(QWidget* parent)
        : QComboBox(parent) {
    setDisabled(true);

    connect(this, &PackageComboBox::listRequested,
            WorkerCore::getInstance(), &WorkerCore::getPackagesList);
    connect(WorkerCore::getInstance(), &WorkerCore::packagesListGot,
            this, &PackageComboBox::onListGot);

    emit listRequested(PackageRequest{
        true, // id
        false
    });
}

void PackageComboBox::setCurrent(int packageId) {
    currentId = packageId;
    for (int i = 0; i < count(); ++i) {
        if (itemData(i) == packageId) {
            setCurrentIndex(i);
            break;
        }
    }
}

// Slots

void PackageComboBox::onListGot(const QVector<Package>& packages) {
    for (const auto& package : packages) {
        addItem(package.name, package.id);
    }
    setCurrent(currentId);
    setDisabled(false);
}
