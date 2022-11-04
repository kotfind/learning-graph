#include "MainWindow.h"
#include "db/db.h"
#include "filesystem/filesystem.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include <QTranslator>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    // For QSettings
    QCoreApplication::setOrganizationName("kotfindApps");
    QCoreApplication::setApplicationName("LearningGraph");

    QSettings settings;
    qDebug().noquote() << QString("Using %1 as settings").arg(settings.fileName());

    // Locale load
    if (!settings.contains("locale")) {
        settings.setValue("locale", "en");
    }

    QTranslator translator;
    if (settings.value("locale").toString() == "ru") {
        if (!translator.load("localeRu")) {
            return 1;
        }
        app.installTranslator(&translator);
    }

    // Db
    if (!db::init()) {
        return 1;
    }

    // Main window
    MainWindow win;
    win.show();

    return app.exec();
}
