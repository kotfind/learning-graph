#pragma once

#include "SmartListWidget.h"

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

class PackageTab : public QWidget {
    Q_OBJECT

    public:
        PackageTab(QWidget* parent = nullptr);

    private:
        void ui();

        QLineEdit* nameEdit;
        QCheckBox* autoUpdateCheckBox;
        QPushButton* updateButton;
        SmartListWidget* packagesList;

    signals:
        void packagesUpdated();
        void themesUpdated();

    public slots:
        void update();
        void setAutoUpdate(bool state);

    private slots:
        void onCreateBtn();
        void packageDoubleClicked(int packageId);
        void packageMenuRequested(int packageId, const QPoint& globalPos);
};
