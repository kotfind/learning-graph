#pragma once

#include <QComboBox>
#include <QVector>

class PackageComboBox : public QComboBox {
    Q_OBJECT

    public:
        PackageComboBox(QWidget* parent = nullptr);
        void setCurrent(int packageId);
        void setAny(bool v);

    private:
        int currentId;
        bool hasOptionAny = false;
};
