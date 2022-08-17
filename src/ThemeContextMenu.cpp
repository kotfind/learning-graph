#include "ThemeContextMenu.h"

#include "ThemeInfoWindow.h"
#include "LearningGraphTab.h"
#include "LearningListTab.h"
#include "GlobalSignalHandler.h"

#include "sqlDefines.h"

#include <QMessageBox>
#include <QSqlQuery>

ThemeContextMenu::ThemeContextMenu(
    int themeId, const QString& themeName, QWidget* parent)
        : QMenu(parent), themeId(themeId), themeName(themeName) {
    ui();

    connect(
        this,
        &ThemeContextMenu::themesUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::themesUpdated
    );
}

void ThemeContextMenu::ui() {
    addAction(tr("Watch/ Edit"), [this](){
        (new ThemeInfoWindow(themeId))->show();
    });
    addAction(tr("Delete"), [this](){
        if (QMessageBox::question(
                this,
                "Question",
                tr("Delete theme \"%1\"?").arg(themeName))
                    == QMessageBox::Yes) {
            QSqlQuery query;
            LOG_PREPARE(query, " \
                DELETE \
                FROM themes \
                WHERE id = ? \
            ")
            query.addBindValue(themeId);
            LOG_EXEC(query)

            emit themesUpdated();
        }
    });
    addSeparator();
    addAction(tr("Build Learning Graph"), [](){
        // TODO
    });
    addAction(tr("Build Learning List"), [](){
        // TODO
    });
}
