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
