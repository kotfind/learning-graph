#include "db.h"
#include "sqlDefines.h"

using namespace db;

void list::build(int themeId) {
    QSqlQuery query;

    // delete old list
    PREPARE(query, " \
        DELETE \
        FROM listThemes \
    ")
    EXEC(query)
    query.finish();

    // XXX: dummy
    PREPARE(query, " \
        INSERT \
        INTO listThemes (themeId) \
            SELECT id \
            FROM themes \
    ")
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

    return theme::reads(ids);
}
