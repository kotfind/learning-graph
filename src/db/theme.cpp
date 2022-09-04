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

void theme::read(
    int id,
    QString* name,
    int* packageId,
    QString* description,
    bool* inWishlist,
    bool* isLearned
    ) {

    PREPARE_NEW(query, " \
        SELECT name, packageId, isLearned, inWishlist, description \
        FROM themes \
        WHERE id = ? \
    ")
    query.addBindValue(id);
    EXEC(query)
    query.first();

     *name = query.value(0).toString();
     *packageId = query.value(1).toInt();
     *isLearned = query.value(2).toBool();
     *inWishlist = query.value(3).toBool();
     *description = query.value(4).toString();
}

int theme::write(
    int id,
    const QString& name,
    int packageId,
    const QString& description,
    bool inWishlist,
    bool isLearned
    ) {

    // Create / update theme
    QSqlQuery query;

    if (id == -1) {
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

    query.bindValue(":name", name);
    query.bindValue(":packageId", packageId);
    query.bindValue(":description", description);
    query.bindValue(":inWishlist", inWishlist);
    query.bindValue(":isLearned", isLearned);
    if (id != -1) {
        query.bindValue(":id", id);
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

    if (id == -1) {
        id = query.lastInsertId().toInt();
    }

    return id;
}
