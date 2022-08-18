#pragma once

#include <QWidget>

class GraphTab : public QWidget {
    Q_OBJECT

    public:
        GraphTab(QWidget* parent = nullptr);

    private:
        void ui();

    signals:
        void graphsUpdated();

    private slots:
        void onCreateBtn();
};
