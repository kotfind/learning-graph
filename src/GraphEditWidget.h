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

        int graphId;

        GraphScene* graphScene;

        QWidget* widget;
        GraphView* graphView;
        QLabel* nameLabel;
        ScaleSpinBox* scaleSpinBox;
        QPushButton* exportButton;

        const QMargins exportMargins = {10, 10, 10, 10};

    public slots:
        void open(int graphId);

    private slots:
        void exportGraph();
};
