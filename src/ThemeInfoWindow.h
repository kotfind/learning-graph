#pragma once

#include "PackageComboBox.h"

#include <QWidget>
#include <QString>
#include <QVector>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include <QKeyEvent>
#include <QPushButton>

class ThemeInfoWindow : public QWidget {
    Q_OBJECT

    public:
        ThemeInfoWindow(int themeId, QWidget* parent = nullptr);

    protected:
        void keyPressEvent(QKeyEvent*) override;

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
        void cancel();
        void save();
};
