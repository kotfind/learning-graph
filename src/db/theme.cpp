#include "db.h"

#include "sqlDefines.h"

#include <QString>
#include <QObject>
#include <QList>
#include <QStringList>

using namespace db;

Theme::Theme() : id(-1) {}

QString theme::name(int id) {
    PREPARE_NEW(query, " \
        SELECT name \
        FROM themes \
        WHERE id = ? \
    ")
    query.addBindValue(id);
    EXEC(query)
    if (!query.next()) {
        return "";
    }
    return query.value(0).toString();
}

QString theme::packageName(int id) {
    PREPARE_NEW(query, " \
        SELECT p.name \
        FROM themes t, packages p \
        WHERE t.id = ? \
          AND t.packageId = p.id \
    ")
    query.addBindValue(id);
    EXEC(query)
    if (!query.next()) {
        return "";
    }
    return query.value(0).toString();
}

Theme theme::read(int id) {
    PREPARE_NEW(query, " \
        SELECT name, packageId, isLearned, inWishlist, description \
        FROM themes \
        WHERE id = ? \
    ")
    query.addBindValue(id);
    EXEC(query)
    query.first();

    Theme t;
    t.id = id;
    t.name = query.value(0).toString();
    t.package = package::read(query.value(1).toInt());
    t.inWishlist = query.value(3).toBool();
    t.isLearned = query.value(2).toBool();
    t.description = query.value(4).toString();
    return t;
}

int theme::write(const Theme& t) {
    QSqlQuery query;

    int id = t.id;

    if (t.id == -1) {
        PREPARE(query, " \
            INSERT \
            INTO themes(name, packageId, description, inWishlist, isLearned) \
            VALUES (NULLIF(:name, ''), :packageId, :description, :inWishlist, :isLearned) \
        ")
    } else {
        PREPARE(query, " \
            UPDATE themes \
            SET name = NULLIF(:name, ''), \
                packageId = :packageId, \
                description = :description, \
                inWishlist = :inWishlist, \
                isLearned = :isLearned \
            WHERE id = :id \
        ")
    }

    query.bindValue(":name", t.name);
    query.bindValue(":packageId", t.package.id);
    query.bindValue(":description", t.description);
    query.bindValue(":inWishlist", t.inWishlist);
    query.bindValue(":isLearned", t.isLearned);
    if (t.id != -1) {
        query.bindValue(":id", t.id);
    }

    if (!query.exec()) {
        switch(ERR_CODE(query)) {
            case SQLITE_CONSTRAINT_UNIQUE:
                throw QObject::tr("Name is not unique.");

            case SQLITE_CONSTRAINT_NOTNULL:
                throw QObject::tr("Name should not be empty.");

            default:
                LOG_FAILED_QUERY(query);
                throw 0;
        }
    }

    return t.id == -1
        ? query.lastInsertId().toInt()
        : t.id;
}

void theme::del(int id) {
    PREPARE_NEW(query, " \
        DELETE \
        FROM themes \
        WHERE id = ? \
    ")
    query.addBindValue(id);
    EXEC(query)
    query.finish();

    PREPARE(query, " \
        DELETE \
        FROM themeEdges \
        WHERE beginId = :themeId \
           OR endId = :themeId \
    ")
    query.bindValue(":themeId", id);
    EXEC(query)
    query.finish();
}

QList<Theme> theme::reads(
    const QString& name,
    int packageId,
    Qt::CheckState inWishlist,
    Qt::CheckState isLearned
    ) {

    QString queryString = " \
        SELECT id, name, packageId, isLearned, inWishlist \
        FROM themes \
        WHERE {whereSection} \
        GROUP BY ( \
            SELECT name \
            FROM packages \
            WHERE id = packageId \
        ), name \
    ";

    // Where secion
    QVector<QVariant> params;

    QString whereSection = "TRUE";
    if (name.size()) {
        whereSection += " AND themes.name LIKE '%' || ? || '%'";
        params.append(name);
    }

    if (packageId != -1) {
        whereSection += " AND packageId = ?";
        params.append(packageId);
    }

    if (inWishlist != Qt::PartiallyChecked) {
        whereSection += QString(" AND inWishlist = ?");
        params.append(inWishlist == Qt::Checked);
    }

    if (isLearned != Qt::PartiallyChecked) {
        whereSection += QString(" AND isLearned = ?");
        params.append(isLearned == Qt::Checked);
    }

    queryString.replace("{whereSection}", whereSection);

    PREPARE_NEW(query, queryString);
    for (const auto& param : params) {
        query.addBindValue(param);
    }
    EXEC(query)

    QList<Theme> themes;
    while (query.next()) {
        Theme t;
        t.id = query.value(0).toInt();
        t.name = query.value(1).toString();
        t.package = package::read(query.value(2).toInt());
        t.inWishlist = query.value(4).toBool();
        t.isLearned = query.value(3).toBool();
        themes.append(t);
    }
    return themes;
}

QList<Theme> theme::reads(int excludeGraphId) {
    PREPARE_NEW(query, " \
        SELECT id, name, packageId, isLearned, inWishlist \
        FROM themes \
        WHERE id NOT IN ( \
            SELECT themeId \
            FROM graphNodes \
            WHERE graphId = ? \
        ) \
        GROUP BY ( \
            SELECT name \
            FROM packages \
            WHERE id = packageId \
        ), name \
    ")
    query.addBindValue(excludeGraphId);
    EXEC(query)

    QList<Theme> themes;
    while (query.next()) {
        Theme t;
        t.id = query.value(0).toInt();
        t.name = query.value(1).toString();
        t.package = package::read(query.value(2).toInt());
        t.inWishlist = query.value(4).toBool();
        t.isLearned = query.value(3).toBool();
        themes.append(t);
    }
    return themes;
}

QList<Theme> theme::reads(const QList<int>& ids) {
    PREPARE_NEW(query, QString(" \
        SELECT id, name, packageId, isLearned, inWishlist \
        FROM themes \
        WHERE id IN ({ids}) \
        GROUP BY ( \
            SELECT name \
            FROM packages \
            WHERE id = packageId \
        ), name \
    ").replace("{ids}", QStringList(QList<QString>(ids.size(), "?")).join(",")))
    for (int id : ids) {
        query.addBindValue(id);
    }
    EXEC(query)

    QList<Theme> themes;
    while (query.next()) {
        Theme t;
        t.id = query.value(0).toInt();
        t.name = query.value(1).toString();
        t.package = package::read(query.value(2).toInt());
        t.inWishlist = query.value(4).toBool();
        t.isLearned = query.value(3).toBool();
        themes.append(t);
    }
    return themes;
}
