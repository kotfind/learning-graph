#include "GraphTab.h"

#include "sqlDefines.h"
#include "GraphListWidget.h"
#include "GlobalSignalHandler.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QSqlQuery>
#include <QMessageBox>

GraphTab::GraphTab(QWidget* parent)
        : QWidget(parent) {
    ui();

    connect(
        this,
        &GraphTab::graphsUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::graphsUpdated
    );
}

void GraphTab::ui() {
    // Layouts
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    // Create Button
    auto* createBtn = new QPushButton(tr("New graph"));
    connect(createBtn, &QPushButton::clicked,
            this, &GraphTab::onCreateBtn);
    hbox->addWidget(createBtn);

    // Import Button
    auto* importBtn = new QPushButton(tr("Import graph"));
    hbox->addWidget(importBtn);

    // Graphs List
    auto* graphsList = new GraphListWidget;
    graphsList->update();
    vbox->addWidget(graphsList);
}

void GraphTab::onCreateBtn() {
    bool ok;
    auto name = QInputDialog::getText(this, tr("New graph"),
        tr("New graph name:"), QLineEdit::Normal, "", &ok).trimmed();
    if (ok) {
        // Add graph
        QSqlQuery query;
        LOG_PREPARE(query, " \
            INSERT \
            INTO graphs(name, xoffset, yoffset) \
            VALUES (NULLIF(?, ''), 0, 0) \
        ")
        query.addBindValue(name);

        if (!query.exec()) {
            auto code = query.lastError().nativeErrorCode().toInt();
            switch(code) {
                case SQLITE_CONSTRAINT_UNIQUE:
                    QMessageBox::critical(
                        this,
                        tr("Error"),
                        tr("Name is not unique.")
                    );
                    return;

                case SQLITE_CONSTRAINT_NOTNULL:
                    QMessageBox::critical(
                        this,
                        tr("Error"),
                        tr("Name should not be empty.")
                    );
                    return;

                default:
                    LOG_FAILED_QUERY(query);
                    break;
            }
        }
        emit graphsUpdated();
    }
}
