#include "PackageComboBox.h"

#include "db/sqlDefines.h"
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

    PREPARE_NEW(query, " \
        SELECT name, id \
        FROM packages \
        ORDER BY name \
    ")
    EXEC(query)

    while (query.next()) {
        addItem(
            query.value(0).toString(),
            query.value(1).toInt()
        );
    }

    setCurrent(currentId);
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
