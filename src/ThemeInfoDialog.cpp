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
#include <QSettings>

ThemeInfoDialog::ThemeInfoDialog(int themeId, QWidget* parent)
        : QDialog(parent), themeId(themeId) {
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
    if (themeId == -1) {
        QSettings settings;
        packageComboBox->setCurrentId(settings.value("themeInfoDialog/packageId").toInt());
    } else {
        const Theme t = db::theme::read(themeId);
        themeEdit->setText(t.name);
        packageComboBox->setCurrentId(t.package.id);
        isLearnedCheckBox->setChecked(t.isLearned);
        inWishlistCheckBox->setChecked(t.inWishlist);
        descriptionEdit->setText(t.description);

        dependsOnList->clear();
        const auto deps = db::theme::readsDependencies(themeId);
        for (const auto& t : deps) {
            dependsOnList->addItem(
                tr("%1 (%2)")
                    .arg(t.name)
                    .arg(t.package.name)
            );
        }
    }
}

void ThemeInfoDialog::ui() {
    setWindowTitle(tr("Theme \"%1\" Info").arg(db::theme::name(themeId)));

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

    createPackageButton = new QPushButton(tr("Create empty package"));
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

    // Depends on list
    auto* dependsOnFrame = new QFrame;
    dependsOnFrame->setFrameShape(QFrame::StyledPanel);
    vbox->addWidget(dependsOnFrame);

    auto* dependsOnBox = new QVBoxLayout;
    dependsOnFrame->setLayout(dependsOnBox);

    auto* dependsOnLabel = new QLabel(tr("Depends on:"));
    dependsOnBox->addWidget(dependsOnLabel);

    dependsOnList = new QListWidget;
    dependsOnList->setSelectionMode(QAbstractItemView::NoSelection);
    dependsOnBox->addWidget(dependsOnList);

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
        packageComboBox->setCurrentId(d.getId());
    }
}

void ThemeInfoDialog::save() {
    // Check package
    if (!packageComboBox->currentData().isValid()) {
        QMessageBox::critical(
            this,
            tr("Error"),
            tr("Package should be selected.")
        );
        return;
    }

    try {
        // Write to db
        Package p;
        p.id = packageComboBox->currentData().toInt();

        Theme t;
        t.id = themeId;
        t.name = themeEdit->text().trimmed();
        t.package = p;
        t.inWishlist = inWishlistCheckBox->isChecked();
        t.isLearned = isLearnedCheckBox->isChecked();
        t.description = descriptionEdit->toPlainText();

        themeId = db::theme::write(t);

        // Write to settings
        QSettings settings;
        settings.setValue("themeInfoDialog/packageId", p.id);

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
