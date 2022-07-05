#include "WorkerCore.h"

#include <QStringList>
#include <QSqlQuery>
#include <QVariant>

QString packageRequestToString(const PackageRequest& fields) {
    QStringList list;

                        list.append("rowid");
    if (fields.name)    list.append("name");

    return list.join(", ");
}

Package queryToPackage(const PackageRequest& fields, const QSqlQuery& query) {
    int idx = 0;

    Package package;
                        package.id      = query.value(idx++).toInt();
    if (fields.name)    package.name    = query.value(idx++).toString();

    return package;
}

void WorkerCore::getPackagesList(
    const PackageRequest& fields) {
    // TODO themeIds
    QSqlQuery query(QString("SELECT %1 FROM packages")
        .arg(packageRequestToString(fields)));
    query.exec();

    QVector<Package> packages;
    while (query.next()) {
        packages.append(queryToPackage(fields, query));
    }
    emit packagesListGot(packages);
}

void WorkerCore::getPackage(
    const PackageRequest& fields,
    int packageId) {
    // TODO themeIds
    QSqlQuery query(QString("SELECT %1 FROM packages WHERE rowid = %2")
            .arg(packageRequestToString(fields))
            .arg(packageId)
            );
    query.exec();
    query.first();
    emit packageGot(queryToPackage(fields, query));
}
