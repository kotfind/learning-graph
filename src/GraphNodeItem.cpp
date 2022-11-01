#include "GraphNodeItem.h"

#include "GraphScene.h"
#include "db/db.h"
#include "GlobalSignalHandler.h"

#include <QDebug>
#include <QMargins>

using namespace db;

GraphNodeItem::GraphNodeItem(int nodeId, QGraphicsItem* parent)
        : QGraphicsTextItem(parent), nodeId(nodeId) {
    setFlag(QGraphicsItem::ItemIsMovable, true);
    load();

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::themesUpdated,
        this,
        &GraphNodeItem::load
    );

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::packagesUpdated,
        this,
        &GraphNodeItem::load
    );

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::fontSet,
        this,
        &GraphNodeItem::setFont
    );

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::fontSet,
        this,
        &GraphNodeItem::positionChanged
    );
}

void GraphNodeItem::paint(
    QPainter* qp,
    const QStyleOptionGraphicsItem* options,
    QWidget* widget) {

    // Pen
    auto pen = qp->pen();
    if (deleted) {
        pen.setStyle(Qt::DashLine);
    }
    qp->setPen(pen);

    // Background
    qp->setBrush(Qt::white);

    // Border
    auto w = qp->pen().width();
    qp->drawRect(boundingRect() - QMargins(w, w, w, w));

    // Text
    QGraphicsTextItem::paint(qp, options, widget);
}

void GraphNodeItem::load() {
    auto node = graphNode::read(nodeId);
    setPos(node.x, node.y);

    if (theme::exists(node.themeId)) {
        auto theme = theme::read(node.themeId);

        setPlainText(
            QString("%1 from %2")
                .arg(theme.name)
                .arg(theme.package.name)
        );

        deleted = false;
    } else {
        setHtml(tr("<i>Deleted</i>"));

        deleted = true;

        emit deleteEdges();
    }
}

void GraphNodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    graphNode::move(nodeId, pos().x(), pos().y());

    QGraphicsTextItem::mouseReleaseEvent(e);
}

void GraphNodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    QGraphicsTextItem::mouseMoveEvent(e);
    emit positionChanged();
}

bool GraphNodeItem::intersect(const QLineF& l1, QPointF* ans) {
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
