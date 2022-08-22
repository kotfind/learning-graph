#include "GraphNode.h"

#include "GraphScene.h"
#include "sqlDefines.h"
#include "GlobalSignalHandler.h"

#include <QDebug>
#include <QMargins>

GraphNode::GraphNode(int nodeId, QGraphicsItem* parent)
        : QGraphicsTextItem(parent), nodeId(nodeId) {
    setFlag(QGraphicsItem::ItemIsMovable, true);
    load();

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::themesUpdated,
        this,
        &GraphNode::load
    );

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::packagesUpdated,
        this,
        &GraphNode::load
    );
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
    QGraphicsTextItem::paint(qp, options, widget);
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
    EXEC(query)
    query.next();

    setPlainText(
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
    EXEC(query)

    QGraphicsTextItem::mouseReleaseEvent(e);
}

void GraphNode::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    QGraphicsTextItem::mouseMoveEvent(e);
    emit positionChanged();
}

bool GraphNode::intersect(const QLineF& l1, QPointF* ans) {
    QList<QPointF> pts = {
        mapToScene(boundingRect().bottomLeft()),
        mapToScene(boundingRect().bottomRight()),
        mapToScene(boundingRect().topRight()),
        mapToScene(boundingRect().topLeft()),
    };
    pts.append(pts.front());

    for (int i = 0; i < 4; ++i) {
        QLineF l2(pts[i], pts[i + 1]);
        if (l1.intersects(l2, ans) == QLineF::BoundedIntersection) {
            return true;
        }
    }
    ans = nullptr;
    return false;
}
