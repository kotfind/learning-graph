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
