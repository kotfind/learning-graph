#include "ThemeInfoWindow.h"
#include "sqlDefines.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>

ThemeInfoWindow::ThemeInfoWindow(int themeId, QWidget* parent)
        : QWidget(parent), themeId(themeId) {
    setWindowTitle(tr("Theme \"%1\" Info").arg(themeId));
    setWindowModality(Qt::ApplicationModal);

    // Main Layout
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    // Theme
    auto* themeFrame = new QFrame;
    themeFrame->setFrameShape(QFrame::StyledPanel);
    vbox->addWidget(themeFrame);

    auto* themeBox = new QVBoxLayout;
    themeFrame->setLayout(themeBox);

    auto* themeLbl = new QLabel(tr("Theme:"));
    themeBox->addWidget(themeLbl);

    themeEdit = new QLineEdit;
    themeBox->addWidget(themeEdit);

    // Package
    auto* packageFrame = new QFrame;
    packageFrame->setFrameShape(QFrame::StyledPanel);
    vbox->addWidget(packageFrame);

    auto* packageBox = new QVBoxLayout;
    packageFrame->setLayout(packageBox);

    auto* packageLbl = new QLabel(tr("Package:"));
    packageBox->addWidget(packageLbl);

    packageCombo = new PackageComboBox;
    packageBox->addWidget(packageCombo);

    // Checkboxes
    auto* checkFrame = new QFrame;
    checkFrame->setFrameShape(QFrame::StyledPanel);
    vbox->addWidget(checkFrame);

    auto* checkBox = new QHBoxLayout;
    checkFrame->setLayout(checkBox);

    isLearnedCheck = new QCheckBox(tr("Learned"));
    checkBox->addWidget(isLearnedCheck);

    inWishlistCheck = new QCheckBox(tr("In Wishlist"));
    checkBox->addWidget(inWishlistCheck);

    checkBox->addStretch(1);

    // Description
    auto* descFrame = new QFrame;
    descFrame->setFrameShape(QFrame::StyledPanel);
    vbox->addWidget(descFrame);

    auto* descBox = new QVBoxLayout;
    descFrame->setLayout(descBox);

    auto* descLbl = new QLabel(tr("Description:"));
    descBox->addWidget(descLbl);

    descEdit = new QTextEdit;
    descBox->addWidget(descEdit);

    // Buttons Layout
    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    hbox->addStretch(1);

    // Cancel Button
    auto* cancelBtn = new QPushButton(tr("Cancel"));
    connect(cancelBtn, &QPushButton::clicked,
            this, &ThemeInfoWindow::close);
    hbox->addWidget(cancelBtn, 0);

    // Save Button
    auto* saveBtn = new QPushButton(themeId == -1 ? tr("Create") : tr("Update"));
    connect(saveBtn, &QPushButton::clicked,
            this, &ThemeInfoWindow::onSaveClicked);
    hbox->addWidget(saveBtn, 0);

    // Load Theme
    if (themeId != -1) {
        QSqlQuery query;
        LOG_PREPARE(query, " \
            SELECT name, package_id, is_learned, in_wishlist, description\
            FROM themes \
            WHERE id = ?")
        query.addBindValue(themeId);
        LOG_EXEC(query)
        query.first();

        themeEdit->setText(query.value(0).toString());
        packageCombo->setCurrent(query.value(1).toInt());
        isLearnedCheck->setChecked(query.value(2).toBool());
        inWishlistCheck->setChecked(query.value(3).toBool());
        descEdit->setText(query.value(4).toString());
    }
}

// Slots

void ThemeInfoWindow::onSaveClicked() {
    // Check package
    if (!packageCombo->currentData().isValid()) {
        QMessageBox::critical(this, tr("Error"), tr("Package should be selected"));
        return;
    }

    // Create / update theme
    QSqlQuery query;

    if (themeId == -1) {
        LOG_PREPARE(query, " \
            INSERT \
            INTO themes(name, package_id, description, in_wishlist, is_learned) \
            VALUES (NULLIF(:name, ''), :package_id, :description, :in_wishlist, :is_learned) \
        ")
    } else {
        LOG_PREPARE(query, " \
            UPDATE themes \
            SET name = NULLIF(:name, ''), \
                package_id = :package_id, \
                description = :description, \
                in_wishlist = :in_wishlist, \
                is_learned = :is_learned \
            WHERE id = :id \
        ")
    }

    query.bindValue(":name", themeEdit->text().trimmed());
    query.bindValue(":package_id", packageCombo->currentData().toInt());
    query.bindValue(":description", descEdit->toPlainText());
    query.bindValue(":in_wishlist", inWishlistCheck->isChecked());
    query.bindValue(":is_learned", isLearnedCheck->isChecked());
    if (themeId != -1) {
        query.bindValue(":id", themeId);
    }

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

    emit close();
}
