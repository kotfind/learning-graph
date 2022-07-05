#include "WorkerCore.h"

#include <QSqlQuery>

void WorkerCore::initDb() {
    QSqlQuery query;

    query.prepare("CREATE TABLE IF NOT EXISTS packages("
        // id is rowid
        "name VARCHAR(255) NOT NULL UNIQUE"
        ")");
    query.exec();
    query.finish();

    query.prepare("CREATE TABLE IF NOT EXISTS themes("
        // id is rowid
        "name VARCHAR(255) NOT NULL,"
        "package_id INT NOT NULL REFERENCES pacakges(rowid),"
        "description TEXT,"
        "in_wishlist INT CHECK(in_wishlist in (0, 1)),"
        "is_learned INT CHECK(is_learned in (0, 1)),"
        "UNIQUE (package_id, name)"
        ")");
    query.exec();
    query.finish();

    query.prepare("CREATE TABLE IF NOT EXISTS theme_dependencies("
        // id is rowid
        "from_id INT NOT NULL REFERENCES themes(rowid),"
        "to_id INT NOT NULL REFERENCES themes(rowid),"
        "UNIQUE (from_id, to_id)"
        ")");
    query.exec();
    query.finish();
}
