#pragma once

#include <QWidget>

class PackagesTab : public QWidget {
    Q_OBJECT

    public:
        PackagesTab(QWidget* parent = nullptr);

    private:
        void ui();

    signals:
        void packagesUpdated();

    private slots:
        void onCreateBtn();
};
