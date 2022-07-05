#include "WorkerCore.h"

#include <QSqlQuery>
#include <QApplication>

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

    query.prepare("CREATE TABLE IF NOT EXISTS themes("
        // id is rowid
        "name VARCHAR(255) NOT NULL,"
        "package_id INT NOT NULL,"
        "description TEXT,"
        "in_wishlist INT CHECK(in_wishlist in (0, 1)),"
        "is_learned INT CHECK(is_learned in (0, 1)),"
        "UNIQUE (package_id, name)"
        ")");
    query.exec();
    query.finish();

    query.prepare("CREATE TABLE IF NOT EXISTS theme_dependencies("
        // id is rowid
        "from_id INT NOT NULL,"
        "to_id INT NOT NULL,"
        "UNIQUE (from_id, to_id)"
        ")");
    query.exec();
    query.finish();

    query.prepare("CREATE TABLE IF NOT EXISTS packages("
        // id is rowid
        "name VARCHAR(255) NOT NULL UNIQUE"
        ")");
    query.exec();
    query.finish();
}

// Slots

void WorkerCore::getThemesList(
    const ThemeRequest& fields,
    const QString& themeNameKeyword,
    int packageId,
    Qt::CheckState inWishlist,
    Qt::CheckState isLearned) {
    QVector<Theme> themes = {
        Theme{1, "Theme 1", 1, "Theme 1 Description", false, false, {}},
        Theme{2, "Theme 2", 2, "Theme 2 Description", false, true, {}},
        Theme{3, "Theme 3", 3, "Theme 3 Description", true, false, {}},
        Theme{4, "Theme 4", 4, "Theme 4 Description", true, true, {}},
    };
    emit themesListGot(themes);
}

void WorkerCore::getTheme(const ThemeRequest& fields, int themeId) {
    emit themeGot(Theme{
        themeId,
        QString("Theme %1").arg(themeId),
        themeId,
        QString("Theme %1 Description").arg(themeId),
        false, false, {}});
}

void WorkerCore::getPackagesList(
    const PackageRequest& fields) {
    QVector<Package> packages = {
        Package{1, "Package 1", {}},
        Package{2, "Package 2", {}},
        Package{3, "Package 3", {}},
        Package{4, "Package 4", {}},
    };
    emit packagesListGot(packages);
}
