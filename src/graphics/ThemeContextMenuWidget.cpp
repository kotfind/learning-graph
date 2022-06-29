#include "ThemeContextMenuWidget.h"
#include "ThemeInfo.h"

ThemeContextMenuWidget::ThemeContextMenuWidget(
    const QString& themeName, QWidget* parent)
        : QMenu(parent), themeName(themeName) {
    addAction(tr("Watch/ Edit"), [&themeName](){
        (new ThemeInfo(themeName))->show();
    });
    addAction(tr("Build Learning Graph"));
    addAction(tr("Build Learning List"));
}
