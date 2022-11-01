#pragma once

#include <QDoubleSpinBox>

class ScaleSpinBox : public QDoubleSpinBox {
    public:
        ScaleSpinBox(QWidget* parent = nullptr);

        // Makes steps exponential
        // NOTE: with exponential steps scaling looks more realistic
        void stepBy(int) override;

        // Returns value double v converted to text
        QString textFromValue(double v) const override;

        // Returns text s converted to double value
        double valueFromText(const QString& s) const override;

    private:
        double stepFactor = 1e-1;
};
