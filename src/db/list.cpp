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

QList<Theme> list::reads() {
    PREPARE_NEW(query, " \
        SELECT themeId \
        FROM listThemes \
    ")
    EXEC(query)

    QList<int> ids;
    while (query.next()) {
        ids.append(query.value(0).toInt());
    }

    return theme::readsByIds(ids, false);
}
