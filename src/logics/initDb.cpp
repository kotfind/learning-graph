#include "WorkerCore.h"
#include "sqlDefines.h"

#include <QSqlQuery>

void WorkerCore::initDb() {
    QSqlQuery query;

    LOG_PREPARE(query, " \
        CREATE TABLE packages( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            name VARCHAR(255) NOT NULL UNIQUE \
        )")
    LOG_EXEC(query)
    query.finish();

    LOG_PREPARE(query, " \
        CREATE TABLE IF NOT EXISTS themes( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            name VARCHAR(255) NOT NULL, \
            package_id INT NOT NULL REFERENCES packages(rowid), \
            description TEXT, \
            in_wishlist INT CHECK(in_wishlist in (0, 1)), \
            is_learned INT CHECK(is_learned in (0, 1)), \
            UNIQUE (package_id, name) \
        )")
    LOG_EXEC(query)
    query.finish();

    LOG_PREPARE(query, " \
        CREATE TABLE theme_dependencies( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            from_id INT NOT NULL REFERENCES themes(rowid), \
            to_id INT NOT NULL REFERENCES themes(rowid), \
            UNIQUE (from_id, to_id) \
        )")
    LOG_EXEC(query)
    query.finish();
}
