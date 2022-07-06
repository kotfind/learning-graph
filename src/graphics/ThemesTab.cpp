#include "ThemesTab.h"
#include "ThemesListWidget.h"
#include "PackageComboBox.h"

#include "../logics/WorkerCore.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>

ThemesTab::ThemesTab(QWidget* parent)
        : QWidget(parent) {
    // Layout
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    // Create Button
    auto* createBtn = new QPushButton(tr("New theme"));
    vbox->addWidget(createBtn);

    // Search section
    auto* searchFrame = new QFrame;
    searchFrame->setFrameShape(QFrame::StyledPanel);
    vbox->addWidget(searchFrame);

    auto* grid = new QGridLayout;
    grid->setColumnStretch(0, 0);
    grid->setColumnStretch(1, 1);
    searchFrame->setLayout(grid);

    // Frame Title
    auto* frameLbl = new QLabel(tr("Search section"));
    grid->addWidget(frameLbl, 0, 0, 1, 2, Qt::AlignCenter);

    // Theme
    auto* themeLbl = new QLabel(tr("Theme:"));
    grid->addWidget(themeLbl, 1, 0, Qt::AlignRight);

    auto* themeEdit = new QLineEdit;
    grid->addWidget(themeEdit, 1, 1);

    // Package
    auto* packageLbl = new QLabel(tr("Package:"));
    grid->addWidget(packageLbl, 2, 0, Qt::AlignRight);

    auto* packageCombo = new PackageComboBox;
    packageCombo->setAny(true);
    grid->addWidget(packageCombo, 2, 1);

    // In Wishlist Switch
    auto* wishlistCheck = new QCheckBox(tr("In Wishlist"));
    wishlistCheck->setTristate(true);
    wishlistCheck->setCheckState(Qt::PartiallyChecked);
    grid->addWidget(wishlistCheck, 4, 1);

    // Learned List Switch
    auto* learnedCheck = new QCheckBox(tr("Learned"));
    learnedCheck->setTristate(true);
    grid->addWidget(learnedCheck, 3, 1);

    // Search Button
    auto* searchBtn = new QPushButton(tr("Search"));
    grid->addWidget(searchBtn, 5, 0, 1, 2);

    // Themes List
    auto* themesList = new ThemesListWidget;
    vbox->addWidget(themesList);


    // Connections
    connect(searchBtn, &QPushButton::clicked, [=]() {
        emit themesList->listRequested(
            themeEdit->text(), 
            packageCombo->currentData().toInt(),
            wishlistCheck->checkState(),
            learnedCheck->checkState());
    });
    connect(themesList, &ThemesListWidget::listRequested,
            WorkerCore::getInstance(), &WorkerCore::getThemesList);
}
