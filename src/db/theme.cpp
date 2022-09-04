#include "db.h"

#include "sqlDefines.h"

#include <QString>

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
