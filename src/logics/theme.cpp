#include "WorkerCore.h"

#include <QStringList>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>

QString formWhere(int packageId, Qt::CheckState inWishlist, Qt::CheckState isLearned) {
    auto res = QString("name LIKE :like");
    if (packageId != -1) {
        res += QString(" AND package_id = %1").arg(packageId);
    }
    if (inWishlist != Qt::PartiallyChecked) {
        res += QString(" AND in_wishlist = %1").arg(inWishlist == Qt::Checked ? 1 : 0);
    }
    if (isLearned != Qt::PartiallyChecked) {
        res += QString(" AND is_learned = %1").arg(isLearned == Qt::Checked ? 1 : 0);
    }
    return res;
}

void WorkerCore::getThemesList(
    const QString& themeNameKeyword,
    int packageId,
    Qt::CheckState inWishlist,
    Qt::CheckState isLearned) {

    QSqlQuery query;

    query.prepare("SELECT rowid, name FROM themes WHERE " +
        formWhere(packageId, inWishlist, isLearned));
    query.bindValue(":like", "%" + themeNameKeyword + "%");
    query.exec();

    QVector<Theme> themes;
    while (query.next()) {
        Theme theme;

        theme.id            = query.value(0).toInt();
        theme.name          = query.value(1).toString();
        // TODO package name

        themes.append(theme);
    }
    emit themesListGot(themes);
}

void WorkerCore::getTheme(int themeId) {
    QSqlQuery query;
    query.prepare("SELECT "
        "rowid, name, package_id, description, in_wishlist, is_learned "
        "FROM themes WHERE rowid = :id");
    query.bindValue(":id", themeId);
    query.exec();
    query.first();

    Theme theme;

    theme.id            = query.value(0).toInt();
    theme.name          = query.value(1).toString();
    theme.package.id    = query.value(2).toInt();
    theme.description   = query.value(3).toString();
    theme.inWishlist    = query.value(4).toBool();
    theme.isLearned     = query.value(5).toBool();

    emit themeGot(theme);
}

bool checkTheme(const Theme& theme) {
    if (theme.name.isEmpty()) {
        emit WorkerCore::getInstance()->errorGot(
            WorkerCore::tr("Name should not be empty."));
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT rowid FROM themes "
        "WHERE package_id = :package_id "
            "AND name = :name "
            "AND NOT rowid = :rowid");
    query.exec();
    if (query.first()) {
        emit WorkerCore::getInstance()->errorGot(
            WorkerCore::tr("Theme \"%1\" already exists.")
                .arg(theme.name));
        return false;
    }
    return true;
}

void WorkerCore::createTheme(const Theme& theme) {
    if (!checkTheme(theme)) {
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO themes"
        "(name, package_id, description, in_wishlist, is_learned) "
        "VALUES "
        "(:name, :package_id, :description, :in_wishlist, :is_learned)");
    query.bindValue(":name",        theme.name);
    query.bindValue(":package_id",  theme.package.id);
    query.bindValue(":description", theme.description);
    query.bindValue(":in_wishlist", theme.inWishlist);
    query.bindValue(":is_learned",  theme.isLearned);
    query.exec();
}

void WorkerCore::updateTheme(const Theme& theme) {
    if (!checkTheme(theme)) {
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE themes "
        "SET name = :name, package_id = :package_id, description = :description, "
            "in_wishlist = :in_wishlist, is_learned = :is_learned "
        "WHERE rowid = :rowid");
    query.bindValue(":rowid",       theme.id);
    query.bindValue(":name",        theme.name);
    query.bindValue(":package_id",  theme.package.id);
    query.bindValue(":description", theme.description);
    query.bindValue(":in_wishlist", theme.inWishlist);
    query.bindValue(":is_learned",  theme.isLearned);
    query.exec();
}

void WorkerCore::deleteTheme(int themeId) {
    QSqlQuery query;
    query.prepare("DELETE FROM themes "
        "WHERE rowid = :rowid");
    query.bindValue(":rowid", themeId);
    query.exec();
}
