#include "GraphTab.h"

#include "sqlDefines.h"
#include "GlobalSignalHandler.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>

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
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::graphsUpdated,
        this,
        &GraphTab::update
    );

    connect(
        graphsList,
        &SmartListWidget::doubleClicked,
        this,
        &GraphTab::graphDoubleClicked
    );

    connect(
        graphsList,
        &SmartListWidget::menuRequested,
        this,
        &GraphTab::graphMenuRequested
    );

    update();
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
    graphsList = new SmartListWidget;
    vbox->addWidget(graphsList);
}

void GraphTab::onCreateBtn() {
    bool ok;
    auto name = QInputDialog::getText(this, tr("New graph"),
        tr("New graph name:"), QLineEdit::Normal, "", &ok).trimmed();
    if (ok) {
        // Add graph
        PREPARE_NEW(query, " \
            INSERT \
            INTO graphs(name, xoffset, yoffset) \
            VALUES (NULLIF(?, ''), 0, 0) \
        ")
        query.addBindValue(name);

        if (!query.exec()) {
            switch(ERR_CODE(query)) {
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
                    return;
            }
        }
        emit graphsUpdated();
    }
}

void GraphTab::update() {
    PREPARE_NEW(query, " \
        SELECT g.id, g.name, ( \
            SELECT COUNT(*) \
            FROM graphNodes n \
            WHERE n.graphId = g.id \
        ) \
        FROM graphs g \
        ORDER BY g.name \
    ")
    EXEC(query)

    graphsList->clear();
    while (query.next()) {
        graphsList->addItem(
            tr("%1 (%2 themes)")
                .arg(query.value(1).toString())
                .arg(query.value(2).toInt()),
            query.value(0).toInt()
        );
    }
}

void GraphTab::graphDoubleClicked(int graphId) {
    emit open(graphId);
}

void GraphTab::graphMenuRequested(int graphId, const QPoint& globalPos) {
    QMenu menu;

    menu.addAction(tr("Open"), [=]() {
        emit open(graphId);
    });

    menu.addAction(tr("Delete"), [=]() {
        if (QMessageBox::question(
                this,
                "Question",
                tr("Delete graph \"%1\"?").arg(graphId)) // TODO: graphId -> graphName
                    == QMessageBox::Yes) {

            QSqlQuery query;
            PREPARE(query, " \
                DELETE \
                FROM graphs \
                WHERE id = ? \
            ")
            query.addBindValue(graphId);
            EXEC(query)
            query.finish();

            PREPARE(query, " \
                DELETE \
                FROM graphNodes \
                WHERE graphId = ? \
            ")
            query.addBindValue(graphId);
            EXEC(query)

            emit graphsUpdated();
        }
    });

    menu.addAction(tr("Rename"), [=]() {
        PREPARE_NEW(query, " \
            SELECT name \
            FROM graphs \
            WHERE id = ? \
        ");
        query.addBindValue(graphId);
        EXEC(query)
        query.next();
        auto oldName = query.value(0).toString();
        query.finish();

        bool ok;
        auto name = QInputDialog::getText(this, tr("Rename graph"),
            tr("Graph name:"), QLineEdit::Normal, oldName, &ok).trimmed();

        if (ok) {
            PREPARE_NEW(query, " \
                UPDATE graphs \
                SET name = ? \
                WHERE id = ? \
            ")
            query.addBindValue(name);
            query.addBindValue(graphId);

            if (!query.exec()) {
                switch(ERR_CODE(query)) {
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
                        return;
                }
            }

            emit graphsUpdated();
        }
    });

    menu.exec(globalPos);
}
