#include "GraphScene.h"

#include "ComboboxIdDialog.h"
#include "sqlDefines.h"
#include "GlobalSignalHandler.h"
#include "ThemeInfoDialog.h"
#include "GraphEdge.h"

#include <QDebug>
#include <QMessageBox>

GraphScene::GraphScene()
        : QGraphicsScene() {
    connect(
        this,
        &GraphScene::graphsUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::graphsUpdated
    );
}

void GraphScene::setMode(GraphEditMode mode) {
    this->mode = mode;
}

void GraphScene::open(int graphId) {
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
        auto* node = new GraphNode(query.value(0).toInt());
        addItem(node);
    }
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    switch (mode) {
        case NEW_NODE_EDIT_MODE:
            newNode(e->scenePos());
            break;

        case ARROW_EDIT_MODE:
            pressedNode = qgraphicsitem_cast<GraphNode*>(itemAt(e->scenePos(), QTransform()));
            break;

        case CURSOR_EDIT_MODE:
            QGraphicsScene::mousePressEvent(e);
            break;
    }
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    switch (mode) {
        case ARROW_EDIT_MODE:
            auto* releasedNode = qgraphicsitem_cast<GraphNode*>(itemAt(e->scenePos(), QTransform()));

            if (!releasedNode || !pressedNode || releasedNode == pressedNode) {
                return;
            }

            auto* edge = new GraphEdge(-1, pressedNode, releasedNode); // FIXME: id
            addItem(edge);

            break;
    }

    QGraphicsScene::mouseReleaseEvent(e);
}

void GraphScene::newNode(QPointF pos) {
    ComboboxIdDialog d((QWidget*)views()[0]);
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
        ThemeInfoDialog d(-1, (QWidget*)views()[0]);

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

    auto* node = new GraphNode(query.lastInsertId().toInt());
    addItem(node);

    emit graphsUpdated();
}
