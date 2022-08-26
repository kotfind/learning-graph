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
        void ui();

        QLineEdit* nameEdit;
        QCheckBox* autoUpdateCheckBox;
        QPushButton* updateButton;
        SmartListWidget* graphsList;

    signals:
        void graphsUpdated();
        void open(int graphId);

    public slots:
        void update();
        void setAutoUpdate(bool state);

    private slots:
        void onCreateBtn();
        void graphDoubleClicked(int graphId);
        void graphMenuRequested(int graphId, const QPoint& globalPos);
};
