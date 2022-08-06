#include "WorkerCore.h"

#include <QSqlQuery>
#include <QApplication>
#include <QFileInfo>

WorkerCore* WorkerCore::instance = nullptr;

WorkerCore::WorkerCore()
        : QObject() {
    bool firstRun = !QFileInfo::exists(dbFilename);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilename);
    if (!db.open()) {
        qApp->exit();
        return;
    }

    if (firstRun) {
        initDb();
    }
}

WorkerCore* WorkerCore::getInstance() {
    if (instance == nullptr) {
        instance = new WorkerCore;
    }

    return instance;
}
