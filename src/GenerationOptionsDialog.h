#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QComboBox>

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

        // Returns value of languageComboBox ("ru" or "en")
        QString getLanguage() { return languageComboBox->currentData().toString(); }

    private:
        // Draws user interface
        void ui();

        QComboBox* languageComboBox;
        QLineEdit* nameEdit;
        QSpinBox* depthLimitSpinBox;
        QSpinBox* quantityLimitSpinBox;
        QPushButton* cancelButton;
        QPushButton* okButton;

        int packageId;
};
