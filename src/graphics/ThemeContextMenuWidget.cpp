#include "ThemeContextMenuWidget.h"
#include "ThemeInfo.h"
#include "LearningGraphWindow.h"
#include "LearningListWindow.h"

ThemeContextMenuWidget::ThemeContextMenuWidget(
    const QString& themeName, QWidget* parent)
        : QMenu(parent), themeName(themeName) {
    addAction(tr("Watch/ Edit"), [&themeName](){
        (new ThemeInfo(themeName))->show();
    });
    addAction(tr("Build Learning Graph"), [&themeName](){
        (new LearningGraphWindow(themeName))->show();
    });
    addAction(tr("Build Learning List"), [&themeName](){
        (new LearningListWindow(themeName))->show();
    });
}
