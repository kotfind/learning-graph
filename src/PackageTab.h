#pragma once

#include <QWidget>

class PackageTab : public QWidget {
    Q_OBJECT

    public:
        PackageTab(QWidget* parent = nullptr);

    private:
        void ui();

    signals:
        void packagesUpdated();

    private slots:
        void onCreateBtn();
};
