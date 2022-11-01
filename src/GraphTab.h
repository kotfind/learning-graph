#pragma once

#include "SmartListWidget.h"

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

class GraphTab : public QWidget {
    Q_OBJECT

    public:
        GraphTab(QWidget* parent = nullptr);

    private:
        // Draws user interface
        void ui();

        QLineEdit* nameEdit;
        QCheckBox* autoUpdateCheckBox;
        QPushButton* updateButton;
        QPushButton* createButton;
        SmartListWidget* graphsList;

    signals:
        void graphsUpdated();
        void open(int graphId);

    public slots:
        // (Re)loads graphsList from db
        void update();

        // Sets autoupdate mode to state
        void setAutoUpdate(bool state);

    private slots:
        // Opens create dialog
        void onCreateButton();

        // Opens graph with id graphId
        void onGraphDoubleClicked(int graphId);

        // Opens graph context menu for graph with id graphId
        void onGraphMenuRequested(int graphId, const QPoint& globalPos);
};
