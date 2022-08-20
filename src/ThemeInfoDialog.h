#pragma once

#include "PackageComboBox.h"

#include <QDialog>
#include <QString>
#include <QVector>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include <QKeyEvent>
#include <QPushButton>

class ThemeInfoDialog : public QDialog {
    Q_OBJECT

    public:
        ThemeInfoDialog(int themeId, QWidget* parent = nullptr);

        int getId();

    private:
        void ui();
        void load();

        int themeId;

        QLineEdit* themeEdit;
        PackageComboBox* packageCombo;
        QCheckBox* isLearnedCheck;
        QCheckBox* inWishlistCheck;
        QTextEdit* descEdit;
        QPushButton* cancelBtn;
        QPushButton* saveBtn;

    signals:
        void themesUpdated();

    private slots:
        void save();
};
