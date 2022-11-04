#include "ComboboxIdDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

ComboboxIdDialog::ComboboxIdDialog(QWidget* parent)
        : QDialog(parent) {
    ui();

    connect(
        okButton,
        &QPushButton::clicked,
        this,
        &QDialog::accept
    );
    okButton->setDefault(true);

    connect(
        cancelButton,
        &QPushButton::clicked,
        this,
        &QDialog::reject
    );
}

void ComboboxIdDialog::ui() {
    auto* vbox = new QVBoxLayout;
    setLayout(vbox);

    label = new QLabel;
    vbox->addWidget(label);

    combobox = new QComboBox;
    vbox->addWidget(combobox);

    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    hbox->addStretch(1);

    cancelButton = new QPushButton(tr("Cancel"));
    hbox->addWidget(cancelButton);

    okButton = new QPushButton(tr("Ok"));
    okButton->setDefault(true);
    hbox->addWidget(okButton);
}

void ComboboxIdDialog::addItem(const QString& name, int id) {
    combobox->addItem(name, id);
}

void ComboboxIdDialog::setLabel(const QString& str) {
    label->setText(str);
}

int ComboboxIdDialog::getId() {
    return combobox->currentData().toInt();
}
