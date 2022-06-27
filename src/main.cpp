#include "graphics/GraphicsCore.h"
#include "graphics/ThemeInfo.h" // TODO: delete me

#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    // Font
    auto font = app.font();
    font.setPointSize(12);
    app.setFont(font);

    // Main window
    GraphicsCore win1;
    win1.show();

    ThemeInfo win2; // TODO: delete me
    win2.show();

    return app.exec();
}
