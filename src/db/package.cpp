#include "db.h"

#include "sqlDefines.h"

#include <QString>
#include <QObject>
#include <QList>

using namespace db;

Package::Package() : id(-1) {}

QString package::name(int id) {
    PREPARE_NEW(query, " \
        SELECT name \
        FROM packages \
        WHERE id = ? \
    ")
    query.addBindValue(id);
    EXEC(query)
    if (!query.next()) {
        return "";
    }
    return query.value(0).toString();
}

int package::count(int id) {
    PREPARE_NEW(query, " \
        SELECT COUNT(*) \
        FROM themes t, packages p \
        WHERE p.id = ? \
          AND t.packageId = p.id \
        GROUP BY p.id \
    ")
    query.addBindValue(id);
    EXEC(query)
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

int package::write(const Package& p) {
    QSqlQuery query;

    int id = p.id;

    if (id == -1) {
        PREPARE(query, " \
            INSERT \
            INTO packages(name) \
            VALUES (NULLIF(:name, '')) \
        ")
    } else {
        PREPARE(query, " \
            UPDATE packages \
            SET name = NULLIF(:name, '') \
            WHERE id = :id \
        ")
    }

    query.bindValue(":name", p.name);
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

QList<Package> package::reads(const QString& name) {
    PREPARE_NEW(query, " \
        SELECT p.id, p.name, ( \
            SELECT COUNT(*) \
            FROM themes t \
            WHERE t.packageId = p.id \
        ) \
        FROM packages p \
        WHERE p.name LIKE ('%' || ? || '%') \
        ORDER BY p.name \
    ")
    query.addBindValue(name);
    EXEC(query)

    QList<Package> packages;
    while (query.next()) {
        Package p;
        p.id = query.value(0).toInt();
        p.name = query.value(1).toString();
        p.count = query.value(2).toInt();
        packages.append(p);
    }
    return packages;
}

Package package::read(int id) {
    PREPARE_NEW(query, " \
        SELECT p.name, COUNT(*) \
        FROM themes t, packages p \
        WHERE p.id = ? \
          AND t.packageId = p.id \
        GROUP BY p.id \
    ")
    query.addBindValue(id);
    EXEC(query)
    if (!query.next()) {
        return Package();
    }
    Package p;
    p.id = id;
    p.name = query.value(0).toString();
    p.count = query.value(1).toInt();
    return p;
}
