#include "WorkerCore.h"
#include "sqlDefines.h"

#include <QStringList>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>

void WorkerCore::getPackagesList() {
    QSqlQuery query(" \
        SELECT * \
        FROM packages\
    ");
    LOG_EXEC(query)

    QVector<Package> packages;
    while (query.next()) {
        Package package;
        
        package.id      = query.value(0).toInt();
        package.name    = query.value(1).toString();

        packages.append(package);
    }
    emit packagesListGot(packages);
}

void WorkerCore::getPackage(int packageId) {
    QSqlQuery query;
    LOG_PREPARE(query, "\
        SELECT * \
        FROM packages \
        WHERE id = ? \
    ")
    query.addBindValue(packageId);
    LOG_EXEC(query)
    query.first();

    Package package;
    package.id      = query.value(0).toInt();
    package.name    = query.value(1).toString();

    emit packageGot(package);
}

bool checkPackage(const Package& package) {
    if (package.name.isEmpty()) {
        emit WorkerCore::getInstance()->errorGot(
            WorkerCore::tr("Name should not be empty."));
        return false;
    }

    QSqlQuery query;
    LOG_PREPARE(query, " \
        SELECT id \
        FROM packages \
        WHERE name = ? \
          AND NOT id = ? \
        ")
    query.addBindValue(package.name);
    query.addBindValue(package.id);
    LOG_EXEC(query)
    if (query.first()) {
        emit WorkerCore::getInstance()->errorGot(
            WorkerCore::tr("Package \"%1\" already exists.").arg(package.name));
        return false;
    }

    return true;
}

void WorkerCore::createPackage(const Package& package) {
    if (!checkPackage(package)) {
        return;
    }

    QSqlQuery query;
    LOG_PREPARE(query, " \
        INSERT \
        INTO packages(name) \
        VALUES (?) \
        ")
    query.addBindValue(package.name);
    LOG_EXEC(query)
    emit packagesChanged();
}

void WorkerCore::updatePackage(const Package& package) {
    if (!checkPackage(package)) {
        return;
    }

    QSqlQuery query;
    LOG_PREPARE(query, " \
        UPDATE packages \
        SET name = ? \
        WHERE id = ? \
    ")
    query.addBindValue(package.name);
    query.addBindValue(package.id);
    LOG_EXEC(query)

    emit packagesChanged();
}

void WorkerCore::deletePackage(int packageId) {
    QSqlQuery query;
    LOG_PREPARE(query, " \
        DELETE FROM packages \
        WHERE id = ? \
    ")
    query.addBindValue(packageId);
    LOG_EXEC(query)

    emit packagesChanged();
}
