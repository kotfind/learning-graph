#pragma once

#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QRectF>

class GraphNodeItem : public QGraphicsTextItem {
    Q_OBJECT

    public:
        enum { Type = UserType + 0 };

        // Construct node with id nodeId
        GraphNodeItem(int nodeId, QGraphicsItem* parent = nullptr);

        int type() const override { return Type; }

        // Draws current node on qp
        void paint(
            QPainter* qp,
            const QStyleOptionGraphicsItem*,
            QWidget *widget = nullptr
        );

        // Returns current node's id
        int getId() const { return nodeId; }

        // Intersects line l1 with current node
        // Returns false if no intersection
        // Returns true otherwise and
        //     sets ans to intersection point
        bool intersect(const QLineF& l1, QPointF* ans);

        // Return true if node is deleted and false otherwise
        bool isDeleted() { return deleted; }

    private:
        int nodeId;
        bool deleted; // is true if node's theme was deleted

    protected:
        // Writes new node's position to db
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;

        // Moves node and emits positionChanged()
        void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;

    signals:
        void positionChanged();
        void deleteEdges();

    private slots:
        // Loads node's data from db
        void load();
};
