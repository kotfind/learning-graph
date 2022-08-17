#pragma once

#include "ThemeListWidget.h"
#include "PackageComboBox.h"

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>

class ThemeTab : public QWidget {
    Q_OBJECT

    public:
        ThemeTab(QWidget* parent = nullptr);

    private:
        void ui();

        ThemeListWidget* themesList;
        QLineEdit* nameEdit;
        PackageComboBox* packageCombo;
        QCheckBox* wishlistCheck;
        QCheckBox* learnedCheck;
        QPushButton* createBtn;
        QPushButton* searchBtn;

    public slots:
        void update();
};
