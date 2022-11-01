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
        // Draws user inteface
        void ui();

        // Loads data from db
        void load();

        int themeId;

        QLineEdit* themeEdit;
        PackageComboBox* packageComboBox;
        QPushButton* createPackageButton;
        QCheckBox* isLearnedCheckBox;
        QCheckBox* inWishlistCheckBox;
        QTextEdit* descriptionEdit;
        QPushButton* cancelButton;
        QPushButton* saveButton;

    signals:
        void themesUpdated();

    private slots:
        // Opens package creation dialog
        void createPackage();

        // Writes theme to db
        void save();
};
