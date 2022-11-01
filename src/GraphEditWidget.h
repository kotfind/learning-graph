#pragma once

#include "GraphScene.h"
#include "GraphEditMode.h"
#include "GraphView.h"
#include "ScaleSpinBox.h"

#include <QMainWindow>
#include <QString>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMargins>
#include <QToolBar>
#include <qtoolbar.h>

class GraphEditWidget : public QMainWindow {
    Q_OBJECT

    public:
        GraphEditWidget(QWidget* parent = nullptr);

    protected:
        // Calls update status
        void mouseMoveEvent(QMouseEvent*) override;

        // Calls update status
        void mousePressEvent(QMouseEvent*) override;

        // Calls update status
        void mouseReleaseEvent(QMouseEvent*) override;

    private:
        // Draws user interface
        void ui();

        // Draws user interface (header)
        void uiHeader();

        // Draws user interface (body)
        void uiBody();

        // Draws user interface (footer)
        void uiFooter();

        // Updates statusBar text according to mouse position
        // Is called by mouse event handlers
        void updateStatus(QMouseEvent*);

        // Exports graphs as .jpg into file "filename"
        void exportAsJpg(const QString& filename);

        // Exports graphs as .png into file "filename"
        void exportAsPng(const QString& filename);

        // Exports graphs as .svg into file "filename"
        void exportAsSvg(const QString& filename);

        // Exports graphs as .graph into file "filename"
        void exportAsGraph(const QString& filename);

        int graphId = -1;

        GraphScene* graphScene;

        QWidget* widget;
        GraphView* graphView;
        QLabel* nameLabel;
        ScaleSpinBox* scaleSpinBox;
        QPushButton* exportButton;
        QPushButton* closeButton;

        QToolBar* modeBar;
        QToolBar* scaleBar;
        QToolBar* otherButtonsBar;

        const QMargins exportMargins = {10, 10, 10, 10};

    public slots:
        // Opens graph with id graphId
        void open(int graphId);

        // Closes graph
        void close();

    private slots:
        // Exports graph
        void exportGraph();

        // Closes current graph if it was deleted
        void onGraphsUpdated();
};
