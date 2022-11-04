#include "db.h"
#include "sqlDefines.h"

#include <QHash>
#include <QList>
#include <QObject>

using namespace db;

enum VertexStatus {
    NOT_VISITED = 0,
    IN_PROGRESS = 1,
    DONE = 2,
};

void topsort(QHash<int, VertexStatus>& status, QList<int>& ans, int vId) {
    status[vId] = IN_PROGRESS;
    const auto dependsOn = theme::getDependenciesIds(vId);
    for (const auto& uId : dependsOn) {
        switch (status[uId]) {
            case NOT_VISITED:
                topsort(status, ans, uId);
                break;

            case IN_PROGRESS:
                throw QObject::tr("Cannot build the list. Dependency cycle found.");
                break;

            case DONE:
                break;
        }
    }
    status[vId] = DONE;
    ans.append(vId);
}

void list::build(int themeId) {
    QSqlQuery query;

    // Delete old list
    PREPARE(query, " \
        DELETE \
        FROM listThemes \
    ")
    EXEC(query)
    query.finish();

    // Build new list
    QHash<int, VertexStatus> status;
    QList<int> ids;
    topsort(status, ids, themeId);

    // Insert list into db
    PREPARE(query, QString(" \
        INSERT \
        INTO listThemes (themeId) \
        VALUES {ids} \
    ").replace("{ids}", QStringList(QList<QString>(ids.size(), "(?)")).join(",")))
    for (int id : ids) {
        query.addBindValue(id);
    }
    EXEC(query)
    query.finish();
}

QList<int> list::getIds() {
    PREPARE_NEW(query, " \
        SELECT themeId \
        FROM listThemes \
    ")
    EXEC(query)

    QList<int> ids;
    while (query.next()) {
        ids.append(query.value(0).toInt());
    }

    return ids;
}

QList<Theme> list::reads() {
    return theme::readsByIds(list::getIds(), false);
}

int list::getMainThemeId() {
    PREPARE_NEW(query, " \
        SELECT themeId \
        FROM listThemes \
        ORDER BY id DESC \
        LIMIT 1 \
    ")
    EXEC(query)
    if (!query.first()) {
        throw 0;
    }
    return query.value(0).toInt();
}

void list::deleteDeletedThemes() {
    PREPARE_NEW(query, " \
        DELETE \
        FROM listThemes \
        WHERE themeId NOT IN ( \
            SELECT id \
            FROM themes \
        ) \
    ")
    EXEC(query)
}

void list::clear() {
    PREPARE_NEW(query, " \
        DELETE \
        FROM listThemes \
    ")
    EXEC(query)
}

bool list::empty() {
    PREPARE_NEW(query, " \
        SELECT id \
        FROM listThemes \
        LIMIT 1 \
    ")
    return !query.next();
}
