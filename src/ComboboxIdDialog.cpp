#include "ComboboxIdDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

ComboboxIdDialog::ComboboxIdDialog(QWidget* parent)
        : QDialog(parent) {
    ui();

    connect(
        okBtn,
        &QPushButton::pressed,
        this,
        &QDialog::accept
    );
    okBtn->setDefault(true);

    connect(
        cancelBtn,
        &QPushButton::pressed,
        this,
        &QDialog::reject
    );
}

void ComboboxIdDialog::ui() {
    auto* vbox = new QVBoxLayout;
    setLayout(vbox);

    lbl = new QLabel;
    vbox->addWidget(lbl);

    combo = new QComboBox;
    vbox->addWidget(combo);

    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    hbox->addStretch(1);

    cancelBtn = new QPushButton("Cancel");
    hbox->addWidget(cancelBtn);

    okBtn = new QPushButton("Ok");
    hbox->addWidget(okBtn);
}

void ComboboxIdDialog::addItem(const QString& name, int id) {
    combo->addItem(name, id);
}

void ComboboxIdDialog::setLabel(const QString& label) {
    lbl->setText(label);
}

int ComboboxIdDialog::getId() {
    return combo->currentData().toInt();
}
