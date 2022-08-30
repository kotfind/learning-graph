#include "ScaleSpinBox.h"

#include <math.h>

ScaleSpinBox::ScaleSpinBox(QWidget* parent) 
        : QDoubleSpinBox(parent) {
    setValue(1);
    setMaximum(10000);
    setMinimum(0.01);
    setDecimals(4);
    setSuffix(" %");
}

void ScaleSpinBox::stepBy(int v) {
    auto val = value() * pow(2, v * stepFactor);
    val = std::min(maximum(),
        std::max(
            minimum(),
            val
        )
    );
    setValue(val);
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
