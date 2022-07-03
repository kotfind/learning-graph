#include "ThemeContextMenuWidget.h"
#include "ThemeInfoWindow.h"
#include "LearningGraphWindow.h"
#include "LearningListWindow.h"

ThemeContextMenuWidget::ThemeContextMenuWidget(
    int themeId, QWidget* parent)
        : QMenu(parent), themeId(themeId) {
    addAction(tr("Watch/ Edit"), [themeId](){
        (new ThemeInfoWindow(themeId))->show();
    });
    addAction(tr("Build Learning Graph"), [&themeId](){
        (new LearningGraphWindow(themeId))->show();
    });
    addAction(tr("Build Learning List"), [&themeId](){
        (new LearningListWindow(themeId))->show();
    });
}
