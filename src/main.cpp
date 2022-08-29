#include "MainWindow.h"
#include "dbLogics.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QSettings>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    // For QSettings
    QCoreApplication::setOrganizationName("kotfindApps");
    QCoreApplication::setApplicationName("LearningGraph");

    QSettings settings;
    qDebug() << settings.fileName();

    // Db
    if (!initDb()) {
        return 1;
    }

    // Main window
    MainWindow win;
    win.show();

    return app.exec();
}
