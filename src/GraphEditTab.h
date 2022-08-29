#pragma once

#include "GraphScene.h"
#include "GraphEditMode.h"
#include "GraphView.h"

#include <QMainWindow>
#include <QString>
#include <QWidget>
#include <QLabel>

class GraphEditTab : public QMainWindow {
    Q_OBJECT

    public:
        GraphEditTab(QWidget* parent = nullptr);

    private:
        void ui();

        void uiHeader();
        void uiBody();
        void uiFooter();

        int graphId;

        GraphScene* graphScene;

        QWidget* widget;
        GraphView* graphView;
        QLabel* nameLabel;

    signals:
        void modeChanged(GraphEditMode mode);

    public slots:
        void open(int graphId);
};
