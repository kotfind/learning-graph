#include "GraphNodeWidget.h"

#include "GraphCanvasWidget.h"
#include "sqlDefines.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

GraphNodeWidget::GraphNodeWidget(int nodeId, QWidget* parent)
        : QFrame(parent), nodeId(nodeId) {
    ui();
    load();
}

void GraphNodeWidget::ui() {
    setFrameShape(QFrame::Box);

    auto* vbox = new QVBoxLayout;
    setLayout(vbox);

    nameLabel = new QLabel;
    vbox->addWidget(nameLabel);
}

void GraphNodeWidget::load() {
    QSqlQuery query;
    LOG_PREPARE(query, " \
        SELECT name, x, y \
        FROM graphNodes, themes \
        WHERE graphNodes.id = ? \
          AND themeId = themes.id \
    ")
    query.addBindValue(nodeId);
    LOG_EXEC(query)
    query.next();

    nameLabel->setText(query.value(0).toString());
    move(
        query.value(1).toInt(),
        query.value(2).toInt()
    );
}

void GraphNodeWidget::mousePressEvent(QMouseEvent* e) {
    dragPoint = e->pos();
}

void GraphNodeWidget::mouseMoveEvent(QMouseEvent* e) {
    if (((GraphCanvasWidget*)parent())->mode/* XXX */ == CURSOR_EDIT_MODE) {
        move(mapToParent(e->pos() - dragPoint));
    }
}

void GraphNodeWidget::mouseReleaseEvent(QMouseEvent* e) {
    QSqlQuery query;
    LOG_PREPARE(query, " \
        UPDATE graphNodes \
        SET x = ?, \
            y = ? \
        WHERE id = ? \
    ")
    query.addBindValue(pos().x());
    query.addBindValue(pos().y());
    query.addBindValue(nodeId);
    LOG_EXEC(query)
}
