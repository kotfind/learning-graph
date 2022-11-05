#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>

class GenerationOptionsDialog : public QDialog {
    Q_OBJECT

    public:
        GenerationOptionsDialog(int packageId, QWidget* parent = nullptr);

    private:
        // Draws user interface
        void ui();

        QLineEdit* linkEdit;
        QSpinBox* depthLimitSpinBox;
        QSpinBox* quantityLimitSpinBox;
        QPushButton* cancelButton;
        QPushButton* okButton;

        int packageId;
};
