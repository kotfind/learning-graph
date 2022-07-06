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

void WorkerCore::createPackage(const QString& name) {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT rowid FROM packages WHERE name = :name");
    selectQuery.bindValue(":name", name);
    selectQuery.exec();
    if (selectQuery.first()) {
        emit errorGot(tr("Package \"%1\" already exists").arg(name));
        return;
    }
    selectQuery.finish();

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO packages(name) VALUES (:name)");
    insertQuery.bindValue(":name", name);
    insertQuery.exec();
    emit packagesChanged();
}
