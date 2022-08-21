#include "GraphNode.h"

#include "GraphScene.h"
#include "sqlDefines.h"
#include "GlobalSignalHandler.h"

#include <QDebug>
#include <QMargins>

GraphNode::GraphNode(int nodeId, QGraphicsItem* parent)
        : QGraphicsSimpleTextItem(parent), nodeId(nodeId) {
    setFlag(QGraphicsItem::ItemIsMovable, true);
    load();

    // connect(
    //     GlobalSignalHandler::getInstance(),
    //     &GlobalSignalHandler::themesUpdated,
    //     this,
    //     &GraphNode::load
    // );

    // connect(
    //     GlobalSignalHandler::getInstance(),
    //     &GlobalSignalHandler::packagesUpdated,
    //     this,
    //     &GraphNode::load
    // );
}

QRectF GraphNode::boundingRect() const {
    return QGraphicsSimpleTextItem::boundingRect() +
        QMargins(margin, margin, margin, margin);
}

void GraphNode::paint(
    QPainter* qp,
    const QStyleOptionGraphicsItem* options,
    QWidget* widget) {

    // Border
    auto w = qp->pen().width();
    auto r = boundingRect();

    r.size().rwidth() -= w;
    r.size().rheight() -= w;

    qp->drawRect(r);

    // Text
    QGraphicsSimpleTextItem::paint(qp, options, widget);
}

void GraphNode::load() {
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

    setText(
        QString("%1 (%2)")
            .arg(query.value(0).toString())
            .arg(query.value(1).toString())
    );
    setPos(
        query.value(2).toInt(),
        query.value(3).toInt()
    );
}

void GraphNode::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
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

    QGraphicsSimpleTextItem::mouseReleaseEvent(e);
}
