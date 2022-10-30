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
        void mouseMoveEvent(QMouseEvent*) override;
        void mousePressEvent(QMouseEvent*) override;
        void mouseReleaseEvent(QMouseEvent*) override;

    private:
        void ui();

        void uiHeader();
        void uiBody();
        void uiFooter();

        void updateStatus(QMouseEvent*);

        void exportAsJpg(const QString& filename);
        void exportAsPng(const QString& filename);
        void exportAsSvg(const QString& filename);
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
        void open(int graphId);
        void close();

    private slots:
        void exportGraph();
};
