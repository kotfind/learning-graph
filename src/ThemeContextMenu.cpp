#include "ThemeContextMenu.h"

#include "ThemeInfoDialog.h"
#include "GraphEditTab.h"
#include "LearningListTab.h"
#include "GlobalSignalHandler.h"

#include "sqlDefines.h"

#include <QMessageBox>

ThemeContextMenu::ThemeContextMenu(int themeId, QWidget* parent)
        : QMenu(parent), themeId(themeId) {
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
        (new ThemeInfoDialog(themeId, this))->exec();
    });
    addAction(tr("Delete"), [this](){
        if (QMessageBox::question(
                this,
                "Question",
                tr("Delete theme \"%1\"?").arg(themeId)) // TODO: themeId -> themeName
                    == QMessageBox::Yes) {
            PREPARE_NEW(query, " \
                DELETE \
                FROM themes \
                WHERE id = ? \
            ")
            query.addBindValue(themeId);
            EXEC(query)
            query.finish();

            PREPARE(query, " \
                DELETE \
                FROM themeEdges \
                WHERE beginId = :themeId \
                   OR endId = :themeId \
            ")
            query.bindValue(":themeId", themeId);
            EXEC(query)
            query.finish();

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
