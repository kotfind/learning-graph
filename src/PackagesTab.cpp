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
        // Add package
        QSqlQuery query;
        LOG_PREPARE(query, " \
            INSERT \
            INTO packages(name) \
            VALUES (NULLIF(?, '')) \
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
    }
}
