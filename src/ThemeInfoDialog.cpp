#include "ThemeInfoDialog.h"

#include "db/sqlDefines.h"
#include "db/db.h"
#include "GlobalSignalHandler.h"
#include "PackageInfoDialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>

using namespace db;

ThemeInfoDialog::ThemeInfoDialog(int themeId, QWidget* parent)
        : QDialog(parent), themeId(themeId) {
    setWindowTitle(tr("Theme \"%1\" Info").arg(theme::name(themeId)));

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
        QString name;
        int packageId;
        QString description;
        bool inWishlist;
        bool isLearned;

        theme::read(themeId,
            &name,
            &packageId,
            &description,
            &inWishlist,
            &isLearned
        );

        themeEdit->setText(name);
        packageCombo->setCurrent(packageId);
        isLearnedCheck->setChecked(isLearned);
        inWishlistCheck->setChecked(inWishlist);
        descEdit->setText(description);
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

    try {
        theme::write(
            themeId,
            themeEdit->text().trimmed(),
            packageCombo->currentData().toInt(),
            descEdit->toPlainText(),
            inWishlistCheck->isChecked(),
            isLearnedCheck->isChecked()
        );
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

    emit themesUpdated();
    emit accept();
}
