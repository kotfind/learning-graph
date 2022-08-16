#pragma once

#include "PackageComboBox.h"

#include <QWidget>
#include <QString>
#include <QVector>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>

class ThemeInfoWindow : public QWidget {
    Q_OBJECT

    public:
        ThemeInfoWindow(int themeId, QWidget* parent = nullptr);

    private:
        int themeId;

        QLineEdit* themeEdit;
        PackageComboBox* packageCombo;
        QCheckBox* isLearnedCheck;
        QCheckBox* inWishlistCheck;
        QTextEdit* descEdit;

    private slots:
        void onSaveClicked();
};
