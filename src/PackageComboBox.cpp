#include "PackageComboBox.h"

#include "db/db.h"
#include "GlobalSignalHandler.h"

#include <QDebug>

PackageComboBox::PackageComboBox(QWidget* parent)
        : QComboBox(parent) {
    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::packagesUpdated,
        this,
        &PackageComboBox::update
    );

    update();
}

void PackageComboBox::update() {
    clear();

    if (hasOptionAny) {
        addItem(tr("<Any>"), -1);
    }

    auto packages = db::package::reads("");
    for (const auto& p : packages) {
        addItem(
            p.name,
            p.id
        );
    }

    setCurrentId(currentId);
}

void PackageComboBox::setCurrentId(int packageId) {
    for (int i = 0; i < count(); ++i) {
        if (itemData(i) == packageId) {
            setCurrentIndex(i);
            currentId = packageId;
            return;
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
