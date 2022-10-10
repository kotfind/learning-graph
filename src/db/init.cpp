#include "sqlDefines.h"
#include "db.h"

#include <QFileInfo>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QStandardPaths>
#include <QDir>

bool createTables() {
    QSqlQuery query;

    // Packages
    PREPARE(query, " \
        CREATE TABLE packages( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            name VARCHAR(255) NOT NULL UNIQUE \
        ) \
    ")
    EXEC(query)
    query.finish();

    // Themes
    PREPARE(query, " \
        CREATE TABLE themes( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            name VARCHAR(255) NOT NULL, \
            packageId INT NOT NULL REFERENCES packages(id), \
            description TEXT, \
            inWishlist INT CHECK(inWishlist in (0, 1)), \
            isLearned INT CHECK(isLearned in (0, 1)), \
            UNIQUE (packageId, name) \
        ) \
    ")
    EXEC(query)
    query.finish();

    // Theme Dependencies
    PREPARE(query, " \
        CREATE TABLE themeEdges( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            beginId INT NOT NULL REFERENCES themes(id), \
            endId INT NOT NULL REFERENCES themes(id), \
            UNIQUE (beginId, endId), \
            CHECK (beginId != endId) \
        ) \
    ")
    EXEC(query)
    query.finish();

    // Graphs
    PREPARE(query, " \
        CREATE TABLE graphs( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            name VARCHAR(255) NOT NULL UNIQUE \
        ) \
    ")
    EXEC(query)
    query.finish();

    // graphNodes
    PREPARE(query, " \
        CREATE TABLE graphNodes( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            graphId INTEGER REFERENCES graphs(id), \
            themeId INTEGER REFERENCES themes(id), \
            x INTEGER NOT NULL, \
            y INTEGER NOT NULL, \
            UNIQUE (graphId, themeId) \
        ) \
    ")
    EXEC(query)
    query.finish();

    return true;
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

bool db::init() {
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
        return createTables();
    }

    return true;
}
