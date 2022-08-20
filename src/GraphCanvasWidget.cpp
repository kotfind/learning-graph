#include "GraphCanvasWidget.h"

#include "ComboboxIdDialog.h"
#include "sqlDefines.h"
#include "GlobalSignalHandler.h"

#include <QDebug>
#include <QMessageBox>

GraphCanvasWidget::GraphCanvasWidget(QWidget* parent)
        : QFrame(parent) {
    connect(
        this,
        &GraphCanvasWidget::graphsUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::graphsUpdated
    );
}

void GraphCanvasWidget::setMode(GraphEditMode mode) {
    this->mode = mode;
}

void GraphCanvasWidget::open(int graphId) {
    this->graphId = graphId;

    PREPARE_NEW(query, " \
        SELECT id \
        FROM graphNodes \
        WHERE graphId = ? \
    ")
    query.addBindValue(graphId);
    LOG_EXEC(query)
    while (query.next()) {
        (new GraphNodeWidget(
            query.value(0).toInt(),
            this
        ))->show();
    }
}

void GraphCanvasWidget::mousePressEvent(QMouseEvent* e) {
    switch (mode) {
        case NEW_NODE_EDIT_MODE:
            newNode(e->pos());
            break;
    }
}

void GraphCanvasWidget::newNode(QPoint pos) {
    ComboboxIdDialog d(this);
    d.setLabel(tr("Choose theme to add:"));

    PREPARE_NEW(query, " \
        SELECT id, name, ( \
            SELECT name \
            FROM packages \
            WHERE id = packageId \
        ) \
        FROM themes \
        WHERE id NOT IN ( \
            SELECT themeId \
            FROM graphNodes \
            WHERE graphId = ? \
        ) \
    ")
    query.addBindValue(graphId);
    LOG_EXEC(query)

    bool queryIsEmpty = true;
    while (query.next()) {
        queryIsEmpty = false;
        d.addItem(
            QString("%1 (%2)")
                .arg(query.value(1).toString())
                .arg(query.value(2).toString()),
            query.value(0).toInt()
        );
    }

    if (queryIsEmpty) {
        QMessageBox::critical(
            this,
            tr("Error"),
            tr("No themes to add.")
        );
        return;
    }

    if (d.exec() == QDialog::Rejected) {
        return;
    }

    auto themeId = d.getId();
    LOG_PREPARE(query, " \
        INSERT \
        INTO graphNodes(graphId, themeId, x, y) \
        VALUES (?, ?, ?, ?) \
    ")
    query.addBindValue(graphId);
    query.addBindValue(themeId);
    query.addBindValue(pos.x());
    query.addBindValue(pos.y());
    LOG_EXEC(query)

    (new GraphNodeWidget(
        query.lastInsertId().toInt(),
        this
    ))->show();

    emit graphsUpdated();
}
