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

        int graphId;

        GraphScene* graphScene;

        QWidget* widget;
        GraphView* graphView;
        QLabel* nameLabel;
        ScaleSpinBox* scaleSpinBox;
        QPushButton* exportButton;

    public slots:
        void open(int graphId);

    private slots:
        void exportGraph();
};
