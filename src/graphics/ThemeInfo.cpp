#include "ThemeInfo.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QPushButton>
#include <QFrame>

ThemeInfo::ThemeInfo(const QString& themeName, QWidget* parent)
        : themeName(themeName), QWidget(parent) {
    setWindowTitle(tr("Theme \"%1\" Info").arg(themeName));

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

    auto* themeEdit = new QLineEdit(themeName);
    themeBox->addWidget(themeEdit);

    // Package
    auto* packageFrame = new QFrame;
    packageFrame->setFrameShape(QFrame::StyledPanel);
    vbox->addWidget(packageFrame);

    auto* packageBox = new QVBoxLayout;
    packageFrame->setLayout(packageBox);

    auto* packageLbl = new QLabel(tr("Package:"));
    packageBox->addWidget(packageLbl);

    auto* packageCombo = new QComboBox;
    packageCombo->addItems({
        "Package 1",
        "Package 2",
        "Package 3",
        "Package 4",
        "Package 5",
    });
    packageBox->addWidget(packageCombo);

    // Checkboxes
    auto* checkFrame = new QFrame;
    checkFrame->setFrameShape(QFrame::StyledPanel);
    vbox->addWidget(checkFrame);

    auto* checkBox = new QHBoxLayout;
    checkFrame->setLayout(checkBox);

    auto* learnedCheck = new QCheckBox(tr("Learned"));
    checkBox->addWidget(learnedCheck);

    auto* inWishlistCheck = new QCheckBox(tr("In Wishlist"));
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

    auto* descEdit = new QTextEdit;
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
}
