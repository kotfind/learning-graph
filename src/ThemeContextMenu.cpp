#include "ThemeContextMenu.h"

#include "ThemeInfoDialog.h"
#include "GraphEditWidget.h"
#include "LearningListWidget.h"
#include "GlobalSignalHandler.h"

#include "db/db.h"

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

    connect(
        this,
        &ThemeContextMenu::openList,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::openList
    );
}

void ThemeContextMenu::ui() {
    addAction(tr("Watch/ Edit"), [this](){
        (new ThemeInfoDialog(themeId, this))->exec();
    });

    addAction(tr("Delete"), [this](){
        if (QMessageBox::question(
                this,
                tr("Question"),
                tr("Delete theme \"%1\"?").arg(db::theme::name(themeId))
            ) == QMessageBox::Yes) {

            db::theme::del(themeId);

            emit themesUpdated();
        }
    });

    addSeparator();

    addAction(tr("Build Learning List"), [this]() {
        emit openList(themeId);
    });
}
