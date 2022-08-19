#pragma once

#include "GraphCanvasWidget.h"
#include "GraphEditMode.h"

#include <QMainWindow>
#include <QString>
#include <QWidget>
#include <QVBoxLayout>
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

        QWidget* widget;
        QVBoxLayout* bodyVBox;
        GraphCanvasWidget* graphFrame;
        QLabel* nameLabel;

    signals:
        void modeChanged(GraphEditMode mode);

    public slots:
        void open(int graphId);
};
