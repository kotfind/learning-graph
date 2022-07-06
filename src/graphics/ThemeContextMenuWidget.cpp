#include "ThemeContextMenuWidget.h"
#include "ThemeInfoWindow.h"
#include "LearningGraphWindow.h"
#include "LearningListWindow.h"

#include "../logics/WorkerCore.h"

#include <QMessageBox>

ThemeContextMenuWidget::ThemeContextMenuWidget(
    int themeId, const QString& themeName, QWidget* parent)
        : QMenu(parent) {
    addAction(tr("Watch/ Edit"), [themeId](){
        (new ThemeInfoWindow(themeId))->show();
    });
    addAction(tr("Delete"), [this, themeId, &themeName](){
        if (QMessageBox::question(this, "Question",
                tr("Delete theme \"%1\"?").arg(themeName))
                == QMessageBox::Yes) {
            emit deleteTheme(themeId);
        }
    });
    addSeparator();
    addAction(tr("Build Learning Graph"), [&themeId](){
        (new LearningGraphWindow(themeId))->show();
    });
    addAction(tr("Build Learning List"), [&themeId](){
        (new LearningListWindow(themeId))->show();
    });

    // Connections
    connect(this, &ThemeContextMenuWidget::deleteTheme,
            WorkerCore::getInstance(), &WorkerCore::deleteTheme);
}
