#include "GraphCanvasWidget.h"

#include "ComboboxIdDialog.h"
#include "sqlDefines.h"

#include <QDebug>

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

    auto* node = new GraphNodeWidget(themeId, this); // FIXME: themeId -> nodeId
    node->move(pos);
    node->show();
}
