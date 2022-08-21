#include "sqlDefines.h"
#include "dbLogics.h"

#include <QFileInfo>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QStandardPaths>
#include <QDir>

void createTables() {
    QSqlQuery query;

    // Packages
    LOG_PREPARE(query, " \
        CREATE TABLE packages( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            name VARCHAR(255) NOT NULL UNIQUE \
        )")
    LOG_EXEC(query)
    query.finish();

    // Themes
    LOG_PREPARE(query, " \
        CREATE TABLE themes( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            name VARCHAR(255) NOT NULL, \
            packageId INT NOT NULL REFERENCES packages(id), \
            description TEXT, \
            inWishlist INT CHECK(inWishlist in (0, 1)), \
            isLearned INT CHECK(isLearned in (0, 1)), \
            UNIQUE (packageId, name) \
        )")
    LOG_EXEC(query)
    query.finish();

    // Theme Dependencies
    LOG_PREPARE(query, " \
        CREATE TABLE themeEdges( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            beginId INT NOT NULL REFERENCES themes(id), \
            endId INT NOT NULL REFERENCES themes(id), \
            UNIQUE (beginId, endId) \
        )")
    LOG_EXEC(query)
    query.finish();

    // Graphs
    LOG_PREPARE(query, " \
        CREATE TABLE graphs( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            name VARCHAR(255) NOT NULL UNIQUE, \
            xoffset INTEGER NOT NULL, \
            yoffset INTEGER NOT NULL \
        )")
    LOG_EXEC(query)
    query.finish();

    // graphNodes
    LOG_PREPARE(query, " \
        CREATE TABLE graphNodes( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            graphId INTEGER REFERENCES graphs(id), \
            themeId INTEGER REFERENCES themes(id), \
            x INTEGER NOT NULL, \
            y INTEGER NOT NULL, \
            UNIQUE (graphId, themeId) \
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
