#pragma once

#include "../logics/Theme.h"
#include "../logics/ThemeRequest.h"

#include "../logics/Package.h"
#include "../logics/PackageRequest.h"

#include <QWidget>
#include <QString>
#include <QVector>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QTextEdit>

class ThemeInfoWindow : public QWidget {
    Q_OBJECT

    public:
        ThemeInfoWindow(int themeId, QWidget* parent = nullptr);

    private:
        int themeId;

        QLineEdit* themeEdit;
        QComboBox* packageCombo;
        QCheckBox* isLearnedCheck;
        QCheckBox* inWishlistCheck;
        QTextEdit* descEdit;

    signals:
        void themeRequest(const ThemeRequest& fields, int themeId);
        void requestPackagesList(
            const PackageRequest& fields,
            const QString& packageNameKeyword);

    private slots:
        void onThemeGot(const Theme& theme);
};
