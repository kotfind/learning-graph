#include "WorkerCore.h"

#include <QStringList>
#include <QSqlQuery>
#include <QVariant>

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
