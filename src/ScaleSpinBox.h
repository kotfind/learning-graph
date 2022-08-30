#pragma once

#include <QDoubleSpinBox>

class ScaleSpinBox : public QDoubleSpinBox {
    public:
        ScaleSpinBox(QWidget* parent = nullptr);

        void stepBy(int) override;

        QString textFromValue(double) const override;
        double valueFromText(const QString&) const override;

    private:
        double stepFactor = 1e-1;
};
