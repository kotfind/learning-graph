#include "WorkerCore.h"

#include <QStringList>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>

void WorkerCore::getPackagesList() {
    QSqlQuery query(QString("SELECT rowid, name FROM packages"));
    query.exec();

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
    query.prepare("SELECT rowid, name FROM packages WHERE rowid = :id");
    query.bindValue(":id", packageId);
    query.exec();
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
    query.prepare("SELECT rowid FROM packages "
        "WHERE name = :name AND NOT rowid = :rowid");
    query.bindValue(":name", package.name);
    query.bindValue(":rowid", package.id);
    query.exec();
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
    query.prepare("INSERT INTO packages(name) VALUES (:name)");
    query.bindValue(":name", package.name);
    query.exec();
    emit packagesChanged();
}

void WorkerCore::updatePackage(const Package& package) {
    if (!checkPackage(package)) {
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE packages "
        "SET name = :name "
        "WHERE rowid = :rowid");
    query.bindValue(":rowid",       package.id);
    query.bindValue(":name",        package.name);
    query.exec();

    emit packagesChanged();
}

void WorkerCore::deletePackage(int packageId) {
    QSqlQuery query;
    query.prepare("DELETE FROM packages "
        "WHERE rowid = :rowid");
    query.bindValue(":rowid", packageId);
    query.exec();

    emit packagesChanged();
}
