#include "sqlDefines.h"
#include "dbLogics.h"

#include <QSqlQuery>
#include <QFileInfo>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QStandardPaths>
#include <QDir>

void createTables() {
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

QString getDbFilename() {
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (path.isEmpty()) {
        return "";
    } else {
        QDir().mkpath(path);
        return path + "/learning_graph.sqlite";
    }
}

bool initDb() {
    auto dbFilename = getDbFilename();
    if (dbFilename.isEmpty()) {
        qDebug() << "Couldn't get database file path";
        return false;
    }
    qDebug() << QString("Using %1 as database").arg(dbFilename);

    bool firstRun = !QFileInfo::exists(dbFilename);

    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilename);
    if (!db.open()) {
        qDebug() << db.lastError();
        return false;
    }

    if (firstRun) {
        createTables();
    }

    return true;
}
