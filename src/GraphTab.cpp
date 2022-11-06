#include "GraphTab.h"

#include "db/db.h"
#include "GlobalSignalHandler.h"
#include "GraphInfoDialog.h"
#include "filesystem/filesystem.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include <QGridLayout>
#include <QLabel>
#include <QFileDialog>
#include <QStandardPaths>

GraphTab::GraphTab(QWidget* parent)
        : QWidget(parent) {
    ui();

    connect(
        this,
        &GraphTab::graphsUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::graphsUpdated
    );

    connect(
        graphsList,
        &SmartListWidget::itemDoubleClicked,
        this,
        &GraphTab::onGraphDoubleClicked
    );

    connect(
        graphsList,
        &SmartListWidget::itemMenuRequested,
        this,
        &GraphTab::onGraphMenuRequested
    );

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::graphsUpdated,
        this,
        &GraphTab::update
    );

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::themesUpdated,
        this,
        &GraphTab::update
    );

    connect(
        updateButton,
        &QPushButton::clicked,
        this,
        &GraphTab::update
    );

    connect(
        autoUpdateCheckBox,
        &QCheckBox::stateChanged,
        [this](int state) {
            setAutoUpdate(state == Qt::Checked);
        }
    );

    connect(
        createButton,
        &QPushButton::clicked,
        this,
        &GraphTab::onCreateButton
    );

    connect(
        importButton,
        &QPushButton::clicked,
        this,
        &GraphTab::onImportButtonClicked
    );

    update();
    setAutoUpdate(true);
    autoUpdateCheckBox->setChecked(true);
}

void GraphTab::ui() {
    // Layouts
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    // Create Button
    createButton = new QPushButton(tr("New graph"));
    hbox->addWidget(createButton);

    // Import Button
    importButton = new QPushButton(tr("Import graph"));
    hbox->addWidget(importButton);

    // Search section
    auto* searchFrame = new QFrame;
    searchFrame->setFrameShape(QFrame::StyledPanel);
    vbox->addWidget(searchFrame);

    auto* grid = new QGridLayout;
    grid->setColumnStretch(0, 0);
    grid->setColumnStretch(1, 1);
    searchFrame->setLayout(grid);

    grid->addWidget(
        new QLabel(tr("Search Section")),
        0, 0,
        1, 2,
        Qt::AlignCenter
    );

    grid->addWidget(
        new QLabel(tr("Name:")),
        1, 0,
        Qt::AlignRight
    );

    nameEdit = new QLineEdit;
    grid->addWidget(nameEdit, 1, 1);

    autoUpdateCheckBox = new QCheckBox(tr("Autoupdate"));
    grid->addWidget(
        autoUpdateCheckBox,
        2, 0
    );

    updateButton = new QPushButton(tr("Search"));
    grid->addWidget(
        updateButton,
        2, 1,
        1, 1,
        Qt::AlignRight
    );

    // Graphs List
    graphsList = new SmartListWidget;
    vbox->addWidget(graphsList);
}

void GraphTab::onCreateButton() {
    GraphInfoDialog d(-1, this);
    d.exec();
}

void GraphTab::update() {
    auto graphs = db::graph::reads(nameEdit->text().trimmed());

    graphsList->clear();
    for (const auto& g : graphs) {
        graphsList->addItem(
            tr("%1 (%2 theme(-s))")
                .arg(g.name)
                .arg(g.count),
            g.id
        );
    }
}

void GraphTab::onGraphDoubleClicked(int graphId) {
    emit open(graphId);
}

void GraphTab::onGraphMenuRequested(int graphId, const QPoint& globalPos) {
    QMenu menu;

    menu.addAction(tr("Open"), [=]() {
        emit open(graphId);
    });

    menu.addAction(tr("Rename"), [=]() {
        GraphInfoDialog d(
            graphId,
            this
        );
        d.exec();
    });

    menu.addAction(tr("Delete"), [=]() {
        if (QMessageBox::question(
                this,
                "Question",
                tr("Delete graph \"%1\"?").arg(db::graph::name(graphId)))
                    == QMessageBox::Yes) {

            db::graph::del(graphId);

            emit graphsUpdated();
        }
    });

    menu.exec(globalPos);
}

void GraphTab::setAutoUpdate(bool state) {
    if (state) {
        updateButton->setDisabled(true);

        connect(
            nameEdit,
            &QLineEdit::textChanged,
            this,
            &GraphTab::update
        );

        connect(
            GlobalSignalHandler::getInstance(),
            &GlobalSignalHandler::graphsUpdated,
            this,
            &GraphTab::update
        );

        update();
    } else {
        updateButton->setDisabled(false);

        disconnect(
            nameEdit,
            &QLineEdit::textChanged,
            this,
            &GraphTab::update
        );

        disconnect(
            GlobalSignalHandler::getInstance(),
            &GlobalSignalHandler::graphsUpdated,
            this,
            &GraphTab::update
        );
    }
}

void GraphTab::onImportButtonClicked() {
    const QString graphFilter = tr("Learning Graph graph (*.graph)");

    auto filename = QFileDialog::getOpenFileName(
        this,
        tr("Import from ..."),
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
        graphFilter
    );

    if (filename.isEmpty()) {
        return;
    }

    try {
        filesystem::graph::importFromGraph(filename);
    } catch (const QString& msg) {
        QMessageBox::critical(
            this,
            tr("Error"),
            msg
        );
        return;
    }

    emit graphsUpdated();
}
