#include "ThemeInfoWindow.h"
#include "../logics/WorkerCore.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

ThemeInfoWindow::ThemeInfoWindow(int themeId, QWidget* parent)
        : QWidget(parent), themeId(themeId) {
    setWindowTitle(tr("Theme \"%1\" Info").arg(themeId));

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

    // Connections
    connect(this, &ThemeInfoWindow::themeRequest,
            WorkerCore::getInstance(), &WorkerCore::getTheme);
    connect(WorkerCore::getInstance(), &WorkerCore::themeGot,
            this, &ThemeInfoWindow::onThemeGot);
    connect(this, &ThemeInfoWindow::creationRequest,
            WorkerCore::getInstance(), &WorkerCore::createTheme);
    connect(this, &ThemeInfoWindow::updateRequest,
            WorkerCore::getInstance(), &WorkerCore::updateTheme);

    if (themeId != -1) {
        setDisabled(true);
        emit themeRequest(themeId);
    }
}

// Slots

void ThemeInfoWindow::onThemeGot(const Theme& theme) {
    themeEdit->setText(theme.name);
    packageCombo->setCurrent(theme.package.id);
    isLearnedCheck->setChecked(theme.isLearned);
    inWishlistCheck->setChecked(theme.inWishlist);
    descEdit->setText(theme.description);
    setDisabled(false);
}

void ThemeInfoWindow::onSaveClicked() {
    Theme theme {
        themeId,
        themeEdit->text().trimmed(),
        Package {
            packageCombo->currentData().toInt(),
            QString()
        },
        descEdit->toPlainText(),
        inWishlistCheck->isChecked(),
        isLearnedCheck->isChecked()
    };

    emit (themeId == -1 ? creationRequest(theme) : updateRequest(theme));
    emit close();
}
