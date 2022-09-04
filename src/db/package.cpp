#include "db.h"

#include "sqlDefines.h"

#include <QString>

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
