#pragma once

#include "../datatypes/Package.h"

#include <QWidget>

class PackagesTab : public QWidget {
    Q_OBJECT

    public:
        PackagesTab(QWidget* parent = nullptr);

    private slots:
        void onCreateBtn();
};
