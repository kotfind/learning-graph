#include "MainWindow.h"
#include "dbLogics.h"

#include <QApplication>
#include <QCoreApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    // For QSettings
    QCoreApplication::setOrganizationName("kotfindApps");
    QCoreApplication::setApplicationName("LearningGraph");

    // Font
    auto font = app.font();
    font.setPointSize(12);
    app.setFont(font);

    // Db
    if (!initDb()) {
        return 1;
    }

    // Main window
    MainWindow win;
    win.show();

    return app.exec();
}
