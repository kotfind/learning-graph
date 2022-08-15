#include "ThemeContextMenuWidget.h"
#include "ThemeInfoWindow.h"
#include "LearningGraphTab.h"
#include "LearningListTab.h"

#include "../logics/sqlDefines.h"

#include <QMessageBox>
#include <QSqlQuery>

ThemeContextMenuWidget::ThemeContextMenuWidget(
    int themeId, const QString& themeName, QWidget* parent)
        : QMenu(parent) {
    addAction(tr("Watch/ Edit"), [themeId](){
        (new ThemeInfoWindow(themeId))->show();
    });
    addAction(tr("Delete"), [this, themeId, &themeName](){
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
        }
    });
    addSeparator();
    addAction(tr("Build Learning Graph"), [&themeId](){
        // TODO
    });
    addAction(tr("Build Learning List"), [&themeId](){
        // TODO
    });
}
