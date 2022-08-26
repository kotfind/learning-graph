#pragma once

#include "SmartListWidget.h"

#include <QWidget>

class PackageTab : public QWidget {
    Q_OBJECT

    public:
        PackageTab(QWidget* parent = nullptr);

    private:
        void ui();

        SmartListWidget* packagesList;

    signals:
        void packagesUpdated();
        void themesUpdated();

    public slots:
        void update();

    private slots:
        void onCreateBtn();
        void packageDoubleClicked(int packageId);
        void packageMenuRequested(int packageId, const QPoint& globalPos);
};
