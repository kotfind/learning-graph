#pragma once

#include "ThemesListWidget.h"
#include "PackageComboBox.h"

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>

class ThemesTab : public QWidget {
    Q_OBJECT

    public:
        ThemesTab(QWidget* parent = nullptr);

    private:
        void ui();

        ThemesListWidget* themesList;
        QLineEdit* nameEdit;
        PackageComboBox* packageCombo;
        QCheckBox* wishlistCheck;
        QCheckBox* learnedCheck;
        QPushButton* createBtn;
        QPushButton* searchBtn;
};
