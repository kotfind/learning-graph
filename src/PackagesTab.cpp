#include "PackagesTab.h"

#include "sqlDefines.h"
#include "PackagesListWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QSqlQuery>
#include <QMessageBox>

PackagesTab::PackagesTab(QWidget* parent)
        : QWidget(parent) {
    // Layouts
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    // Create Button
    auto* createBtn = new QPushButton(tr("New package"));
    connect(createBtn, &QPushButton::clicked,
            this, &PackagesTab::onCreateBtn);
    hbox->addWidget(createBtn);

    // Import Button
    auto* importBtn = new QPushButton(tr("Import package"));
    hbox->addWidget(importBtn);

    // Packages List
    auto* packagesList = new PackagesListWidget;
    vbox->addWidget(packagesList);
}

void PackagesTab::onCreateBtn() {
    bool ok;
    auto name = QInputDialog::getText(this, tr("New package"),
        tr("New package name:"), QLineEdit::Normal, "", &ok).trimmed();
    if (ok) {
        // Check package
        if (name.isEmpty()) {
            QMessageBox::critical(this, tr("Error"), tr("Name should not be empty"));
            return;
        }

        QSqlQuery query;
        LOG_PREPARE(query, " \
            SELECT 1 \
            FROM packages \
            WHERE name = ? \
            ")
        query.addBindValue(name);
        LOG_EXEC(query)

        if (query.first()) {
            QMessageBox::critical(this, tr("Error"), tr("Package \"%1\" already exists.").arg(name));
            return;
        }

        query.finish();

        // Add package
        LOG_PREPARE(query, " \
            INSERT \
            INTO packages(name) \
            VALUES (?) \
        ")
        query.addBindValue(name);
        LOG_EXEC(query)
    }
}
