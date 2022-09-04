#include "db.h"

#include "sqlDefines.h"

#include <QString>
#include <QObject>

using namespace db;

QString package::name(int id) {
    R_PREPARE_NEW(query, " \
        SELECT name \
        FROM packages \
        WHERE id = ? \
    ", "")
    query.addBindValue(id);
    R_EXEC(query, "")
    if (!query.next()) {
        return "";
    }
    return query.value(0).toString();
}

int package::count(int id) {
    R_PREPARE_NEW(query, " \
        SELECT COUNT(*) \
        FROM themes t, packages p \
        WHERE p.id = ? \
          AND t.packageId = p.id \
        GROUP BY p.id \
    ", -1)
    query.addBindValue(id);
    R_EXEC(query, -1)
    if (!query.next()) {
        return -1;
    }
    return query.value(0).toInt();
}

void package::del(int id) {
    QSqlQuery query;

    PREPARE(query, " \
        DELETE \
        FROM packages \
        WHERE id = ? \
    ")
    query.addBindValue(id);
    EXEC(query)
    query.finish();

    PREPARE(query, " \
        DELETE \
        FROM themes \
        WHERE packageId = ? \
    ")
    query.addBindValue(id);
    EXEC(query)
}

int package::write(int id, const QString& name) {
    QSqlQuery query;

    if (id == -1) {
        R_PREPARE(query, " \
            INSERT \
            INTO packages(name) \
            VALUES (NULLIF(:name, '')) \
        ", -1)
    } else {
        R_PREPARE(query, " \
            UPDATE packages \
            SET name = NULLIF(:name, '') \
            WHERE id = :id \
        ", -1)
    }

    query.bindValue(":name", name);
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
