#include "ThemeInfoWindow.h"
#include "../logics/WorkerCore.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

ThemeInfoWindow::ThemeInfoWindow(int themeId, QWidget* parent)
        : QWidget(parent), themeId(themeId) {
    setWindowTitle(tr("Theme \"%1\" Info").arg(themeId));
    setDisabled(true);

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

    packageCombo = new QComboBox;
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

    // Edit Button
    auto* editBtn = new QPushButton(tr("Cancel"));
    hbox->addWidget(editBtn, 0);

    // Ok Button
    auto* okBtn = new QPushButton(tr("Ok"));
    hbox->addWidget(okBtn, 0);

    // Connects
    connect(this, &ThemeInfoWindow::themeRequest,
            WorkerCore::getInstance(), &WorkerCore::getTheme);
    connect(WorkerCore::getInstance(), &WorkerCore::themeGot,
            this, &ThemeInfoWindow::onThemeGot);

    emit themeRequest(ThemeRequest(), themeId); // XXX
}

// Slots

void ThemeInfoWindow::onThemeGot(const Theme& theme) {
    themeEdit->setText(theme.name);
    isLearnedCheck->setChecked(theme.isLearned);
    inWishlistCheck->setChecked(theme.inWishlist);
    descEdit->setText(theme.description);

    connect(WorkerCore::getInstance(), &WorkerCore::packagesListGot,
            [this,&theme](const QVector<Package>& packages) {
        for (const auto& package : packages) {
            this->packageCombo->addItem(package.name, package.id);
            if (theme.packageId == package.id) {
                this->packageCombo->setCurrentIndex(packageCombo->count() - 1);
            }
        }
        this->setDisabled(false);
    });

    connect(this, &ThemeInfoWindow::requestPackagesList,
            WorkerCore::getInstance(), &WorkerCore::getPackagesList);
    emit requestPackagesList(PackageRequest(), "");
}
