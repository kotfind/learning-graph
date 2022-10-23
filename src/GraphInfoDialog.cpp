#include "GraphInfoDialog.h"

#include "GlobalSignalHandler.h"
#include "db/db.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>

using namespace db;

GraphInfoDialog::GraphInfoDialog(int graphId, QWidget* parent)
        : QDialog(parent), graphId(graphId) {
    setWindowTitle(graphId != -1
        ? tr("Graph \"%1\" Info").arg(graph::name(graphId))
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
        cancelBtn,
        &QPushButton::pressed,
        this,
        &GraphInfoDialog::reject
    );

    connect(
        saveBtn,
        &QPushButton::pressed,
        this,
        &GraphInfoDialog::save
    );
}

void GraphInfoDialog::ui() {
    // Main Layout
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    // Name
    auto* nameLbl = new QLabel("Graph name:");
    vbox->addWidget(nameLbl);

    nameEdit = new QLineEdit;
    vbox->addWidget(nameEdit);

    // Buttons
    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    hbox->addStretch(1);

    cancelBtn = new QPushButton("Cancel");
    hbox->addWidget(cancelBtn);

    saveBtn = new QPushButton(graphId == -1 ? tr("Create") : tr("Update"));
    saveBtn->setDefault(true);
    hbox->addWidget(saveBtn);
}

void GraphInfoDialog::load() {
    if (graphId != -1) {
        nameEdit->setText(graph::name(graphId));
    }
}

void GraphInfoDialog::save() {
    try {
        Graph p;
        p.id = graphId;
        p.name = nameEdit->text().trimmed();
        graphId = graph::write(p);
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