#pragma once

#include <QListWidget>
#include <QVector>
#include <QPoint>

class GraphListWidget : public QListWidget {
    Q_OBJECT

    public:
        GraphListWidget(QWidget* parent = nullptr);

    signals:
        void graphsUpdated();

    public slots:
        void update();

    private slots:
        void showContextMenu(const QPoint& pos);
};