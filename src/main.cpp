#include "MainWindow.h"
#include "db/db.h"

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
    qDebug() << settings.fileName();

    // Locale load
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
