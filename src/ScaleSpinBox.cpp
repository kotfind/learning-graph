#include "ScaleSpinBox.h"

#include <math.h>

ScaleSpinBox::ScaleSpinBox(QWidget* parent) 
        : QDoubleSpinBox(parent) {
    setValue(1);
    setMaximum(1000);
    setMinimum(0.0001);
    setDecimals(4);
    setSuffix(" %");
}

void ScaleSpinBox::stepBy(int v) {
    setValue(value() * pow(2, v * stepFactor));
}

QString ScaleSpinBox::textFromValue(double v) const {
    return QString::number(v * 100, 'f', decimals() - 2);
}

double ScaleSpinBox::valueFromText(const QString& s) const {
    auto splited = s.split(' ', Qt::SkipEmptyParts);
    if (splited.empty()) {
        return 1;
    }
    return splited[0].toDouble() / 100;
}
