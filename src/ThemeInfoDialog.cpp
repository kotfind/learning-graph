#include "ThemeInfoDialog.h"

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
        createPackageButton,
        &QPushButton::clicked,
        this,
        &ThemeInfoDialog::createPackage
    );

    connect(
        cancelButton,
        &QPushButton::clicked,
        this,
        &ThemeInfoDialog::reject
    );

    connect(
        saveButton,
        &QPushButton::clicked,
        this,
        &ThemeInfoDialog::save
    );
}

int ThemeInfoDialog::getId() {
    return themeId;
}

void ThemeInfoDialog::load() {
    if (themeId != -1) {
        const Theme t = theme::read(themeId);
        themeEdit->setText(t.name);
        packageComboBox->setCurrent(t.package.id);
        isLearnedCheckBox->setChecked(t.isLearned);
        inWishlistCheckBox->setChecked(t.inWishlist);
        descriptionEdit->setText(t.description);
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

    packageComboBox = new PackageComboBox;
    packageBox->addWidget(packageComboBox);

    createPackageButton = new QPushButton(tr("New package"));
    packageBox->addWidget(createPackageButton);

    // Checkboxes
    auto* checkFrame = new QFrame;
    checkFrame->setFrameShape(QFrame::StyledPanel);
    vbox->addWidget(checkFrame);

    auto* checkBox = new QHBoxLayout;
    checkFrame->setLayout(checkBox);

    isLearnedCheckBox = new QCheckBox(tr("Learned"));
    checkBox->addWidget(isLearnedCheckBox);

    inWishlistCheckBox = new QCheckBox(tr("In Wishlist"));
    checkBox->addWidget(inWishlistCheckBox);

    checkBox->addStretch(1);

    // Description
    auto* descFrame = new QFrame;
    descFrame->setFrameShape(QFrame::StyledPanel);
    vbox->addWidget(descFrame);

    auto* descBox = new QVBoxLayout;
    descFrame->setLayout(descBox);

    auto* descLbl = new QLabel(tr("Description:"));
    descBox->addWidget(descLbl);

    descriptionEdit = new QTextEdit;
    descBox->addWidget(descriptionEdit);

    // Buttons Layout
    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    hbox->addStretch(1);

    // Cancel Button
    cancelButton = new QPushButton(tr("Cancel"));
    hbox->addWidget(cancelButton, 0);

    // Save Button
    saveButton = new QPushButton(themeId == -1 ? tr("Create") : tr("Update"));
    saveButton->setDefault(true);
    hbox->addWidget(saveButton, 0);
}

void ThemeInfoDialog::createPackage() {
    PackageInfoDialog d(-1, this);

    if (d.exec() == QDialog::Accepted) {
        packageComboBox->setCurrent(d.getId());
    }
}

void ThemeInfoDialog::save() {
    // Check package
    if (!packageComboBox->currentData().isValid()) {
        QMessageBox::critical(
            this,
            tr("Error"),
            tr("Package should be selected")
        );
        return;
    }

    try {
        Package p;
        p.id = packageComboBox->currentData().toInt();

        Theme t;
        t.id = themeId;
        t.name = themeEdit->text().trimmed();
        t.package = p;
        t.inWishlist = inWishlistCheckBox->isChecked();
        t.isLearned = isLearnedCheckBox->isChecked();
        t.description = descriptionEdit->toPlainText();

        themeId = theme::write(t);
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
