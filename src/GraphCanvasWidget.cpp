#include "GraphCanvasWidget.h"

#include "ComboboxIdDialog.h"
#include "sqlDefines.h"

#include <QDebug>
#include <QMessageBox>

GraphCanvasWidget::GraphCanvasWidget(QWidget* parent)
        : QFrame(parent) {
}

void GraphCanvasWidget::setMode(GraphEditMode mode) {
    this->mode = mode;
}

void GraphCanvasWidget::open(int graphId) {
    this->graphId = graphId;
    // TODO
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

    QSqlQuery query;
    LOG_PREPARE(query, " \
        SELECT id, name, ( \
            SELECT name \
            FROM packages \
            WHERE id = packageId \
        ) \
        FROM themes \
    ")
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
    LOG_PREPARE(query, " \
        INSERT \
        INTO graphNodes(graphId, themeId, x, y) \
        VALUES (?, ?, ?, ?) \
    ")
    query.addBindValue(graphId);
    query.addBindValue(themeId);
    query.addBindValue(pos.x());
    query.addBindValue(pos.y());

    if (!query.exec()) {
        switch (ERR_CODE(query)) {
            case SQLITE_CONSTRAINT_UNIQUE:
                QMessageBox::critical(
                    this,
                    tr("Error"),
                    tr("This theme exists in graph.")
                );
                return;

            default:
                LOG_FAILED_QUERY(query)
        }
    }

    (new GraphNodeWidget(
        query.lastInsertId().toInt(),
        this
    ))->show();
}
