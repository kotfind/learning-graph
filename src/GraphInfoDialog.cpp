#include "GraphInfoDialog.h"

#include "GlobalSignalHandler.h"
#include "db/db.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>

GraphInfoDialog::GraphInfoDialog(int graphId, QWidget* parent)
        : QDialog(parent), graphId(graphId) {
    setWindowTitle(graphId != -1
        ? tr("Graph \"%1\" Info").arg(db::graph::name(graphId))
        : tr("New Graph")
    );

    ui();
    load();

    connect(
        this,
        &GraphInfoDialog::graphsUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::graphsUpdated
    );

    connect(
        cancelButton,
        &QPushButton::clicked,
        this,
        &GraphInfoDialog::reject
    );

    connect(
        saveButton,
        &QPushButton::clicked,
        this,
        &GraphInfoDialog::save
    );
}

void GraphInfoDialog::ui() {
    // Main Layout
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    // Name
    auto* nameLbl = new QLabel(tr("Graph name:"));
    vbox->addWidget(nameLbl);

    nameEdit = new QLineEdit;
    vbox->addWidget(nameEdit);

    // Buttons
    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    hbox->addStretch(1);

    cancelButton = new QPushButton(tr("Cancel"));
    hbox->addWidget(cancelButton);

    saveButton = new QPushButton(graphId == -1 ? tr("Create") : tr("Update"));
    saveButton->setDefault(true);
    hbox->addWidget(saveButton);
}

void GraphInfoDialog::load() {
    if (graphId != -1) {
        nameEdit->setText(db::graph::name(graphId));
    }
}

void GraphInfoDialog::save() {
    try {
        Graph p;
        p.id = graphId;
        p.name = nameEdit->text().trimmed();
        graphId = db::graph::write(p);
    } catch (const QString& msg) {
        QMessageBox::critical(
            this,
            tr("Error"),
            msg
        );
        return;
    } catch (...) {
        return;
    }

    emit graphsUpdated();
    emit accept();
}
