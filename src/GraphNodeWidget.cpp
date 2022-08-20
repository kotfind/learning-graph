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
    PREPARE_NEW(query, " \
        SELECT t.name, p.name, n.x, n.y \
        FROM graphNodes n, themes t, packages p \
        WHERE n.id = ? \
          AND n.themeId = t.id \
          AND p.id = t.packageId \
    ")
    query.addBindValue(nodeId);
    LOG_EXEC(query)
    query.next();

    nameLabel->setText(
        QString("%1 (%2)")
            .arg(query.value(0).toString())
            .arg(query.value(1).toString())
    );
    move(
        query.value(2).toInt(),
        query.value(3).toInt()
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
    PREPARE_NEW(query, " \
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
