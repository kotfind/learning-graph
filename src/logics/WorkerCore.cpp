#include "WorkerCore.h"

#include <QSqlQuery>
#include <QApplication>

WorkerCore* WorkerCore::instance = nullptr;

WorkerCore::WorkerCore()
        : QObject() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilename);
    if (!db.open()) {
        qApp->exit();
        return;
    }
    initDb();
}

WorkerCore* WorkerCore::getInstance() {
    if (instance == nullptr) {
        instance = new WorkerCore;
    }

    return instance;
}
