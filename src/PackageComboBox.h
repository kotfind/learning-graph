#pragma once

#include <QComboBox>
#include <QVector>

class PackageComboBox : public QComboBox {
    Q_OBJECT

    public:
        PackageComboBox(QWidget* parent = nullptr);

        // Sets current item to package with id packageId
        // Won't change anything this package isn't in combobox
        void setCurrentId(int packageId);

        // Adds option "<Any>" if v == true
        // Deletes it otherwise
        void setAny(bool v);

    private:
        int currentId;

        bool hasOptionAny = false;

    public slots:
        // (Re)loads data from db
        void update();
};
