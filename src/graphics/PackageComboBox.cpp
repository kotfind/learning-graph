#include "PackageComboBox.h"
#include "../logics/WorkerCore.h"

#include <QDebug>

PackageComboBox::PackageComboBox(QWidget* parent)
        : QComboBox(parent) {
    setDisabled(true);

    connect(this, &PackageComboBox::listRequested,
            WorkerCore::getInstance(), &WorkerCore::getPackagesList);
    connect(WorkerCore::getInstance(), &WorkerCore::packagesListGot,
            this, &PackageComboBox::onListGot);
    connect(WorkerCore::getInstance(), &WorkerCore::packagesChanged,
            this, &PackageComboBox::onPackagesChanged);

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

void PackageComboBox::setAny(bool v) {
    if (hasOptionAny == v) {
        return;
    }
    hasOptionAny = v;

    if (hasOptionAny) {
        insertItem(0, tr("<Any>"), -1);
    } else {
        removeItem(0);
    }
}

// Slots

void PackageComboBox::onListGot(const QVector<Package>& packages) {
    clear();
    if (hasOptionAny) {
        addItem(tr("<Any>"), -1);
    }
    for (const auto& package : packages) {
        addItem(package.name, package.id);
    }
    setCurrent(currentId);
    setDisabled(false);
}

void PackageComboBox::onPackagesChanged() {
    emit listRequested(PackageRequest{
        true, // id
        false
    });
}
