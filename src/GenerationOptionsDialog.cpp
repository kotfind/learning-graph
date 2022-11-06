#include "GenerationOptionsDialog.h"

#include "db/db.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QDebug>

GenerationOptionsDialog::GenerationOptionsDialog(int packageId, QWidget* parent)
        : QDialog(parent), packageId(packageId) {
    ui();

    connect(
        cancelButton,
        &QPushButton::clicked,
        this,
        &GenerationOptionsDialog::reject
    );

    connect(
        okButton,
        &QPushButton::clicked,
        this,
        &GenerationOptionsDialog::accept
    );
}

void GenerationOptionsDialog::ui() {
    // Title
    setWindowTitle(
        tr("Generation Options for Package \"%1\"")
           .arg(db::package::name(packageId))
    );

    // Layout
    auto grid = new QGridLayout;
    setLayout(grid);

    // Label
    grid->addWidget(new QLabel(
        tr("Generation Options for Package \"%1\"")
            .arg(db::package::name(packageId))
    ), 1, 1, 1, 2, Qt::AlignHCenter);

    // Name of article
    grid->addWidget(new QLabel(
        tr("Name of Wikipedia article:")
    ), 2, 1, 1, 2, Qt::AlignLeft);

    nameEdit = new QLineEdit;
    grid->addWidget(nameEdit, 3, 1, 1, 2);

    // Language
    grid->addWidget(new QLabel(
        tr("Language of Wikipedia:")
    ), 4, 1, Qt::AlignRight);

    languageComboBox = new QComboBox;
    languageComboBox->addItem(tr("English"), "en");
    languageComboBox->addItem(tr("Russian"), "ru");
    grid->addWidget(languageComboBox, 4, 2);

    // Depth Limit SpinBox
    grid->addWidget(new QLabel(
        tr("Limit of Generation Depth:")
    ), 5, 1, Qt::AlignRight);

    depthLimitSpinBox = new QSpinBox;
    depthLimitSpinBox->setMaximum(1);
    depthLimitSpinBox->setMaximum(50);
    depthLimitSpinBox->setValue(4);
    grid->addWidget(depthLimitSpinBox, 5, 2);

    // Quantity Limit SpinBox
    grid->addWidget(new QLabel(
        tr("Limit of Themes Quantity:")
    ), 6, 1, Qt::AlignRight);

    quantityLimitSpinBox = new QSpinBox;
    quantityLimitSpinBox->setMaximum(1);
    quantityLimitSpinBox->setMaximum(10000);
    quantityLimitSpinBox->setValue(20);
    grid->addWidget(quantityLimitSpinBox, 6, 2);

    // Buttons
    auto* hbox = new QHBoxLayout;
    grid->addLayout(hbox, 7, 1, 1, 2);

    hbox->addStretch(1);

    cancelButton = new QPushButton(tr("Cancel"));
    hbox->addWidget(cancelButton);

    okButton = new QPushButton(tr("Ok"));
    okButton->setDefault(true);
    hbox->addWidget(okButton);
}
