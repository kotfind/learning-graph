#include "db.h"

#include "sqlDefines.h"

#include <QString>
#include <QObject>
#include <QList>
#include <QStringList>

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
        throw 0;
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
        throw 0;
    }
    return query.value(0).toInt();
}

void package::del(int id) {
    QSqlQuery query;

    PREPARE(query, " \
        WITH themesToDeleteIds AS ( \
            SELECT id \
            FROM themes \
            WHERE packageId = ? \
        ) \
        DELETE \
        FROM themeEdges \
        WHERE beginId IN themesToDeleteIds \
           OR   endId IN themesToDeleteIds \
    ")
    query.addBindValue(id);
    EXEC(query)

    PREPARE(query, " \
        DELETE \
        FROM themes \
        WHERE packageId = ? \
    ")
    query.addBindValue(id);
    EXEC(query)

    PREPARE(query, " \
        DELETE \
        FROM packages \
        WHERE id = ? \
    ")
    query.addBindValue(id);
    EXEC(query)
    query.finish();
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
        throw 0;
    }
    Package p;
    p.id = id;
    p.name = query.value(0).toString();
    p.count = query.value(1).toInt();
    return p;
}

QList<int> package::getThemeIds(const QList<int>& packageIds) {
    PREPARE_NEW(query, QString(" \
        SELECT id \
        FROM themes \
        WHERE packageId IN ({ids}) \
    ").replace("{ids}", QStringList(QList<QString>(packageIds.size(), "?")).join(",")))
    for (int id : packageIds) {
        query.addBindValue(id);
    }
    EXEC(query)

    QList<int> ans;
    while (query.next()) {
        ans.append(query.value(0).toInt());
    }
    return ans;
}

QList<Package> package::readsByIds(const QList<int>& ids) {
    PREPARE_NEW(query, QString(" \
        SELECT p.id, p.name, ( \
            SELECT COUNT(*) \
            FROM themes t \
            WHERE t.packageId = p.id \
        ) \
        FROM packages p \
        WHERE id IN ({ids}) \
        ORDER BY p.name \
    ").replace("{ids}", QStringList(QList<QString>(ids.size(), "?")).join(",")))
    for (int id : ids) {
        query.addBindValue(id);
    }
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

bool package::unique(const QString& name) {
    PREPARE_NEW(query, " \
        SELECT 1 \
        FROM packages \
        WHERE name = ? \
    ")
    query.addBindValue(name);
    EXEC(query)
    return !query.first();
}

bool package::hasTheme(int packageId, const QString& name) {
    PREPARE_NEW(query, " \
        SELECT id \
        FROM themes \
        WHERE packageId = ? \
          AND name = ? \
        LIMIT 1 \
    ")
    query.addBindValue(packageId);
    query.addBindValue(name);
    EXEC(query)
    return query.next();
}
