#pragma once

#include <QDialog>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

class ComboboxIdDialog : public QDialog {
    Q_OBJECT

    public:
        ComboboxIdDialog(QWidget* parent = nullptr);

        // Sets label text to param
        void setLabel(const QString&);

        // Adds item with current name and id to combobox
        void addItem(const QString& name, int id);

        // Returns id of current item
        int getId();

    private:
        // Draws user interface
        void ui();

        QLabel* label;
        QComboBox* combobox;
        QPushButton* cancelButton;
        QPushButton* okButton;
};
