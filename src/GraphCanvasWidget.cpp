#include "GraphCanvasWidget.h"

#include "ComboboxIdDialog.h"
#include "sqlDefines.h"
#include "GlobalSignalHandler.h"
#include "ThemeInfoDialog.h"

#include <QDebug>
#include <QMessageBox>

GraphCanvasWidget::GraphCanvasWidget(QWidget* parent)
        : QFrame(parent) {
    setDisabled(true);

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

    clear();

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

    setDisabled(false);
}

void GraphCanvasWidget::clear() {
    auto nodes = findChildren<GraphNodeWidget*>();
    for (auto* node : nodes) {
        node->deleteLater();
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
    d.addItem(tr("<New Node>"), -1);

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

    while (query.next()) {
        d.addItem(
            QString("%1 (%2)")
                .arg(query.value(1).toString())
                .arg(query.value(2).toString()),
            query.value(0).toInt()
        );
    }

    if (d.exec() == QDialog::Rejected) {
        return;
    }

    auto themeId = d.getId();

    if (themeId == -1) {
        ThemeInfoDialog d(-1, this);

        if (d.exec() == QDialog::Rejected) {
            return;
        }

        themeId = d.getId();
    }

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
