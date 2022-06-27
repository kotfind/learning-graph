#include "graphics/GraphicsCore.h"

#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    // Font
    auto font = app.font();
    font.setPointSize(12);
    app.setFont(font);

    // Main window
    GraphicsCore win;
    win.show();

    return app.exec();
}
