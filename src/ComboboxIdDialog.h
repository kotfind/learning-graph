#pragma once

#include <QDialog>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

class ComboboxIdDialog : public QDialog {
    public:
        ComboboxIdDialog(QWidget* parent = nullptr);

        void setLabel(const QString&);
        void addItem(const QString& name, int id);

        int getId();

    private:
        void ui();

        QLabel* lbl;
        QComboBox* combo;
        QPushButton* cancelBtn;
        QPushButton* okBtn;
};
