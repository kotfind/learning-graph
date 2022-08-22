#include "GraphListWidget.h"

#include "sqlDefines.h"
#include "GlobalSignalHandler.h"

#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>

GraphListWidget::GraphListWidget(QWidget* parent)
        : QListWidget(parent) {
    setSelectionMode(QListWidget::NoSelection);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(
        this,
        &GraphListWidget::customContextMenuRequested,
        this,
        &GraphListWidget::showContextMenu
    );

    connect(
        this,
        &GraphListWidget::graphsUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::graphsUpdated
    );

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::graphsUpdated,
        this,
        &GraphListWidget::update
    );
}

void GraphListWidget::update() {
    clear();

    PREPARE_NEW(query, " \
        SELECT g.id, g.name, ( \
            SELECT COUNT(*) \
            FROM graphNodes n \
            WHERE n.graphId = g.id \
        ) \
        FROM graphs g \
        ORDER BY g.name \
    ")
    LOG_EXEC(query)

    while (query.next()) {
        auto name = tr("%1 (%2 themes)")
            .arg(query.value(1).toString())
            .arg(query.value(2).toInt());
        auto* item = new QListWidgetItem(name);
        item->setData(Qt::UserRole, query.value(0));
        addItem(item);
    }
}

void GraphListWidget::showContextMenu(const QPoint& pos) {
    auto* curr = currentItem();
    if (!curr) {
        return;
    }

    QMenu menu;

    menu.addAction(tr("Open"), [=]() {
        emit open(curr->data(Qt::UserRole).toInt());
    });

    menu.addAction(tr("Delete"), [=]() {
        if (QMessageBox::question(
                this,
                "Question",
                tr("Delete graph \"%1\"?").arg(curr->text()))
                    == QMessageBox::Yes) {

            auto graphId = curr->data(Qt::UserRole).toInt();

            QSqlQuery query;
            LOG_PREPARE(query, " \
                DELETE \
                FROM graphs \
                WHERE id = ? \
            ")
            query.addBindValue(graphId);
            LOG_EXEC(query)
            query.finish();

            LOG_PREPARE(query, " \
                DELETE \
                FROM graphNodes \
                WHERE graphId = ? \
            ")
            query.addBindValue(graphId);
            LOG_EXEC(query)

            emit graphsUpdated();
        }
    });

    menu.addAction(tr("Rename"), [=]() {
        auto graphId = curr->data(Qt::UserRole).toInt();

        PREPARE_NEW(query, " \
            SELECT name \
            FROM graphs \
            WHERE id = ? \
        ");
        query.addBindValue(graphId);
        LOG_EXEC(query)
        query.next();
        auto oldName = query.value(0).toString();
        query.finish();

        bool ok;
        auto name = QInputDialog::getText(this, tr("Rename graph"),
            tr("Graph name:"), QLineEdit::Normal, oldName, &ok).trimmed();

        if (ok) {
            PREPARE_NEW(query, " \
                UPDATE graphs \
                SET name = ? \
                WHERE id = ? \
            ")
            query.addBindValue(name);
            query.addBindValue(graphId);

            if (!query.exec()) {
                switch(ERR_CODE(query)) {
                    case SQLITE_CONSTRAINT_UNIQUE:
                        QMessageBox::critical(
                            this,
                            tr("Error"),
                            tr("Name is not unique.")
                        );
                        return;

                    case SQLITE_CONSTRAINT_NOTNULL:
                        QMessageBox::critical(
                            this,
                            tr("Error"),
                            tr("Name should not be empty.")
                        );
                        return;

                    default:
                        LOG_FAILED_QUERY(query);
                        break;
                }
            }

            emit graphsUpdated();
        }
    });

    menu.exec(mapToGlobal(pos));
}
