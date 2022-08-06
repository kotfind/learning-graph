#include "WorkerCore.h"
#include "sqlDefines.h"

#include <QStringList>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QVector>
#include <QSqlError>

void WorkerCore::getThemesList(
    const QString& themeNameKeyword,
    int packageId,
    Qt::CheckState inWishlist,
    Qt::CheckState isLearned) {

    QString queryString = " \
           SELECT id, name \
           FROM themes \
           WHERE TRUE";
    QVector<QVariant> params;

    if (themeNameKeyword.size()) {
        queryString += " AND name LIKE ?";
        params.append("%" + themeNameKeyword + "%");
    }

    if (packageId != -1) {
        queryString += " AND package_id = ?";
        params.append(packageId);
    }

    if (inWishlist != Qt::PartiallyChecked) {
        queryString += QString(" AND in_wishlist = ?");
        params.append(inWishlist == Qt::Checked);
    }

    if (isLearned != Qt::PartiallyChecked) {
        queryString += QString(" AND is_learned = ?");
        params.append(isLearned == Qt::Checked);
    }

    QSqlQuery query;
    LOG_PREPARE(query, queryString);
    for (const auto& param : params) {
        query.addBindValue(param);
    }
    LOG_EXEC(query)

    QVector<Theme> themes;
    while (query.next()) {
        Theme theme;
        theme.id    = query.value(0).toInt();
        theme.name  = query.value(1).toString();

        themes.append(theme);
    }
    emit themesListGot(themes);
}

void WorkerCore::getTheme(int themeId) {
    QSqlQuery query;
    LOG_PREPARE(query, " \
        SELECT * \
        FROM themes \
        WHERE id = ?")
    query.addBindValue(themeId);
    LOG_EXEC(query)
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
    LOG_PREPARE(query, " \
        SELECT rowid FROM themes \
        WHERE package_id = ? \
          AND name = ? \
          AND NOT id = ? \
    ")
    query.addBindValue(theme.package.id);
    query.addBindValue(theme.name);
    query.addBindValue(theme.id);
    LOG_EXEC(query)

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
    LOG_PREPARE(query, " \
        INSERT \
        INTO themes(name, package_id, description, in_wishlist, is_learned) \
        VALUES (?, ?, ?, ?, ?) \
    ")
    query.addBindValue(theme.name);
    query.addBindValue(theme.package.id);
    query.addBindValue(theme.description);
    query.addBindValue(theme.inWishlist);
    query.addBindValue(theme.isLearned);
    LOG_EXEC(query)

    emit themesChanged();
}

void WorkerCore::updateTheme(const Theme& theme) {
    if (!checkTheme(theme)) {
        return;
    }

    QSqlQuery query;
    LOG_PREPARE(query, " \
        UPDATE themes \
        SET name = ?, \
            package_id = ?, \
            description = ?, \
            in_wishlist = ?, \
            is_learned = ? \
        WHERE id = ? \
    ")
    query.addBindValue(theme.name);
    query.addBindValue(theme.package.id);
    query.addBindValue(theme.description);
    query.addBindValue(theme.inWishlist);
    query.addBindValue(theme.isLearned);
    query.addBindValue(theme.id);
    LOG_EXEC(query)

    emit themesChanged();
}

void WorkerCore::deleteTheme(int themeId) {
    QSqlQuery query;
    LOG_PREPARE(query, " \
        DELETE \
        FROM themes \
        WHERE id = ? \
    ")
    query.addBindValue(themeId);
    LOG_EXEC(query)

    emit themesChanged();
}
