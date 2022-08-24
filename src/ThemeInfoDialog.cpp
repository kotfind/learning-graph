#include "ThemeInfoDialog.h"

#include "sqlDefines.h"
#include "GlobalSignalHandler.h"
#include "PackageInfoDialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>

ThemeInfoDialog::ThemeInfoDialog(int themeId, QWidget* parent)
        : QDialog(parent), themeId(themeId) {
    setWindowTitle(tr("Theme \"%1\" Info").arg(themeId)); // TODO: themeId -> themeName

    ui();
    load();

    connect(
        this,
        &ThemeInfoDialog::themesUpdated,
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::themesUpdated
    );

    connect(
        createPackageBtn,
        &QPushButton::pressed,
        this,
        &ThemeInfoDialog::createPackage
    );

    connect(
        cancelBtn,
        &QPushButton::pressed,
        this,
        &ThemeInfoDialog::reject
    );

    connect(
        saveBtn,
        &QPushButton::pressed,
        this,
        &ThemeInfoDialog::save
    );
}

int ThemeInfoDialog::getId() {
    return themeId;
}

void ThemeInfoDialog::load() {
    if (themeId != -1) {
        PREPARE_NEW(query, " \
            SELECT name, packageId, isLearned, inWishlist, description \
            FROM themes \
            WHERE id = ? \
        ")
        query.addBindValue(themeId);
        EXEC(query)
        query.first();

        themeEdit->setText(query.value(0).toString());
        packageCombo->setCurrent(query.value(1).toInt());
        isLearnedCheck->setChecked(query.value(2).toBool());
        inWishlistCheck->setChecked(query.value(3).toBool());
        descEdit->setText(query.value(4).toString());
    }
}

void ThemeInfoDialog::ui() {
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

    createPackageBtn = new QPushButton(tr("New package"));
    packageBox->addWidget(createPackageBtn);

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
    cancelBtn = new QPushButton(tr("Cancel"));
    hbox->addWidget(cancelBtn, 0);

    // Save Button
    saveBtn = new QPushButton(themeId == -1 ? tr("Create") : tr("Update"));
    saveBtn->setDefault(true);
    hbox->addWidget(saveBtn, 0);
}

void ThemeInfoDialog::createPackage() {
    PackageInfoDialog d(-1, this);

    if (d.exec() == QDialog::Accepted) {
        packageCombo->setCurrent(d.getId());
    }
}

void ThemeInfoDialog::save() {
    // Check package
    if (!packageCombo->currentData().isValid()) {
        QMessageBox::critical(
            this,
            tr("Error"),
            tr("Package should be selected")
        );
        return;
    }

    // Create / update theme
    QSqlQuery query;

    if (themeId == -1) {
        PREPARE(query, " \
            INSERT \
            INTO themes(name, packageId, description, inWishlist, isLearned) \
            VALUES (NULLIF(:name, ''), :packageId, :description, :inWishlist, :isLearned) \
        ")
    } else {
        PREPARE(query, " \
            UPDATE themes \
            SET name = NULLIF(:name, ''), \
                packageId = :packageId, \
                description = :description, \
                inWishlist = :inWishlist, \
                isLearned = :isLearned \
            WHERE id = :id \
        ")
    }

    query.bindValue(":name", themeEdit->text().trimmed());
    query.bindValue(":packageId", packageCombo->currentData().toInt());
    query.bindValue(":description", descEdit->toPlainText());
    query.bindValue(":inWishlist", inWishlistCheck->isChecked());
    query.bindValue(":isLearned", isLearnedCheck->isChecked());
    if (themeId != -1) {
        query.bindValue(":id", themeId);
    }

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

    if (themeId == -1) {
        themeId = query.lastInsertId().toInt();
    }

    emit themesUpdated();
    emit accept();
}
