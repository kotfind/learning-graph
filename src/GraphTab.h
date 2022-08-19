#pragma once

#include "GraphListWidget.h"

#include <QWidget>

class GraphTab : public QWidget {
    Q_OBJECT

    public:
        GraphTab(QWidget* parent = nullptr);

    private:
        void ui();

        GraphListWidget* graphsList;

    signals:
        void graphsUpdated();
        void open(int graphId);

    private slots:
        void onCreateBtn();
};
