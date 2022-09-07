#include "db.h"

#include "sqlDefines.h"

#include <QString>
#include <QObject>

using namespace db;

QString theme::name(int id) {
    R_PREPARE_NEW(query, " \
        SELECT name \
        FROM themes \
        WHERE id = ? \
    ", "")
    query.addBindValue(id);
    R_EXEC(query, "")
    if (!query.next()) {
        return "";
    }
    return query.value(0).toString();
}

QString theme::packageName(int id) {
    R_PREPARE_NEW(query, " \
        SELECT p.name \
        FROM themes t, packages p \
        WHERE t.id = ? \
          AND t.packageId = p.id \
    ", "")
    query.addBindValue(id);
    R_EXEC(query, "")
    if (!query.next()) {
        return "";
    }
    return query.value(0).toString();
}

Theme theme::read(int id) {
    const Theme FICTIVE = Theme{-1, "", -1, false, false, ""};
    R_PREPARE_NEW(query, " \
        SELECT name, packageId, isLearned, inWishlist, description \
        FROM themes \
        WHERE id = ? \
    ", FICTIVE)
    query.addBindValue(id);
    R_EXEC(query, FICTIVE)
    query.first();

    return Theme {
        .id = id,
        .name = query.value(0).toString(),
        .packageId = query.value(1).toInt(),
        .inWishlist = query.value(3).toBool(),
        .isLearned = query.value(2).toBool(),
        .description = query.value(4).toString()
    };
}

int theme::write(const Theme& t) {
    QSqlQuery query;

    if (t.id == -1) {
        R_PREPARE(query, " \
            INSERT \
            INTO themes(name, packageId, description, inWishlist, isLearned) \
            VALUES (NULLIF(:name, ''), :packageId, :description, :inWishlist, :isLearned) \
        ", -1)
    } else {
        R_PREPARE(query, " \
            UPDATE themes \
            SET name = NULLIF(:name, ''), \
                packageId = :packageId, \
                description = :description, \
                inWishlist = :inWishlist, \
                isLearned = :isLearned \
            WHERE id = :id \
        ", -1)
    }

    query.bindValue(":name", t.name);
    query.bindValue(":packageId", t.packageId);
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

QList<ThemeForList> theme::readForList(
    const QString& name,
    int packageId,
    Qt::CheckState inWishlist,
    Qt::CheckState isLearned
    ) {

    QString queryString = " \
        SELECT id, name, (\
            SELECT name \
            FROM packages \
            WHERE packageId = id \
        ) \
        FROM themes \
        WHERE {whereSection} \
        GROUP BY ( \
            SELECT name \
            FROM packages \
            WHERE id = packageId \
        ), name \
    ";
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

    R_PREPARE_NEW(query, queryString, {});
    for (const auto& param : params) {
        query.addBindValue(param);
    }
    R_EXEC(query, {})

    QList<ThemeForList> themes;
    while (query.next()) {
        themes.append(ThemeForList{
            .id = query.value(0).toInt(),
            .name = query.value(1).toString(),
            .packageName = query.value(2).toString()
        });
    }
    return themes;
}
