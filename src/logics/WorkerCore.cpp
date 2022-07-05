#include "WorkerCore.h"

#include <QSqlQuery>
#include <QApplication>
#include <QStringList>

WorkerCore* WorkerCore::instance = nullptr;

WorkerCore::WorkerCore()
        : QObject() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilename);
    if (!db.open()) {
        qApp->exit();
        return;
    }
    initDb();
}

WorkerCore* WorkerCore::getInstance() {
    if (instance == nullptr) {
        instance = new WorkerCore;
    }

    return instance;
}

void WorkerCore::initDb() {
    QSqlQuery query;

    query.prepare("CREATE TABLE IF NOT EXISTS packages("
        // id is rowid
        "name VARCHAR(255) NOT NULL UNIQUE"
        ")");
    query.exec();
    query.finish();

    query.prepare("CREATE TABLE IF NOT EXISTS themes("
        // id is rowid
        "name VARCHAR(255) NOT NULL,"
        "package_id INT NOT NULL REFERENCES pacakges(rowid),"
        "description TEXT,"
        "in_wishlist INT CHECK(in_wishlist in (0, 1)),"
        "is_learned INT CHECK(is_learned in (0, 1)),"
        "UNIQUE (package_id, name)"
        ")");
    query.exec();
    query.finish();

    query.prepare("CREATE TABLE IF NOT EXISTS theme_dependencies("
        // id is rowid
        "from_id INT NOT NULL REFERENCES themes(rowid),"
        "to_id INT NOT NULL REFERENCES themes(rowid),"
        "UNIQUE (from_id, to_id)"
        ")");
    query.exec();
    query.finish();
}

// Theme

QString themeRequestToString(const ThemeRequest& fields) {
    QStringList list;

                            list.append("rowid");
    if (fields.name)        list.append("name");
                            list.append("package_id");
    if (fields.description) list.append("description");
    if (fields.inWishlist)  list.append("in_wishlist");
    if (fields.isLearned)   list.append("is_learned");

    return list.join(", ");
}

Theme queryToTheme(const ThemeRequest& fields, const QSqlQuery& query) {
    int idx = 0;

    Theme theme;
                            theme.id            = query.value(idx++).toInt();
    if (fields.name)        theme.name          = query.value(idx++).toString();
                            theme.package.id    = query.value(idx++).toInt();
    if (fields.description) theme.description   = query.value(idx++).toString();
    if (fields.inWishlist)  theme.inWishlist    = query.value(idx++).toBool();
    if (fields.isLearned)   theme.isLearned     = query.value(idx++).toBool();

    return theme;
}

void WorkerCore::getThemesList(
    const ThemeRequest& fields,
    const QString& themeNameKeyword,
    int packageId,
    Qt::CheckState inWishlist,
    Qt::CheckState isLearned) {
    // TODO dependsOn

    QSqlQuery query(QString("SELECT %1 FROM themes") // TODO WHERE
        .arg(themeRequestToString(fields)));
    query.exec();

    QVector<Theme> themes;
    while (query.next()) {
        themes.append(queryToTheme(fields, query));
    }
    emit themesListGot(themes);
}

void WorkerCore::getTheme(const ThemeRequest& fields, int themeId) {
    QSqlQuery query(QString("SELECT %1 FROM themes WHERE rowid = %2")
        .arg(themeRequestToString(fields))
        .arg(themeId)
        );
    query.exec();
    query.first();
    emit themeGot(queryToTheme(fields, query));
}

// Package

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
