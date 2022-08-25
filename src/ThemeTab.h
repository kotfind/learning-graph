#pragma once

#include "PackageComboBox.h"
#include "SmartListWidget.h"

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QKeyEvent>

class ThemeTab : public QWidget {
    Q_OBJECT

    public:
        ThemeTab(QWidget* parent = nullptr);

    private:
        void ui();

        SmartListWidget* themesList;
        QLineEdit* nameEdit;
        PackageComboBox* packageCombo;
        QCheckBox* wishlistCheck;
        QCheckBox* learnedCheck;
        QCheckBox* autoUpdateCheck;
        QPushButton* createBtn;
        QPushButton* searchBtn;

    protected:
        void keyPressEvent(QKeyEvent*) override;

    public slots:
        void update();
        void setAutoUpdate(bool state);

    private slots:
        void themeDoubleClicked(int themeId);
        void themeMenuRequested(int themeId, const QPoint& globalPos);
};
