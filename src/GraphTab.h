#pragma once

#include "SmartListWidget.h"

#include <QWidget>

class GraphTab : public QWidget {
    Q_OBJECT

    public:
        GraphTab(QWidget* parent = nullptr);

    private:
        void ui();

        SmartListWidget* graphsList;

    signals:
        void graphsUpdated();
        void open(int graphId);

    public slots:
        void update();

    private slots:
        void onCreateBtn();
        void graphDoubleClicked(int graphId);
        void graphMenuRequested(int graphId, const QPoint& globalPos);
};
