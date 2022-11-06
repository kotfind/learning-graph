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

        // Returns value of depthLimitSpinBox
        int getDepthLimit() { return depthLimitSpinBox->value(); }

        // Returns value of quantityLimitSpinBox
        int getQuantityLimit() { return quantityLimitSpinBox->value(); }

        // Returns value of nameEdit
        QString getName() { return nameEdit->text(); }

    private:
        // Draws user interface
        void ui();

        QLineEdit* nameEdit;
        QSpinBox* depthLimitSpinBox;
        QSpinBox* quantityLimitSpinBox;
        QPushButton* cancelButton;
        QPushButton* okButton;

        int packageId;
};
