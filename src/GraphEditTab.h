#pragma once

#include "GraphCanvasWidget.h"
#include "GraphEditMode.h"

#include <QMainWindow>
#include <QString>
#include <QWidget>
#include <QVBoxLayout>

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

    signals:
        void modeChanged(GraphEditMode mode);
};
