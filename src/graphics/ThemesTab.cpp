#include "ThemesTab.h"
#include "ThemesListWidget.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
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

    auto* packageCombo = new QComboBox;
    packageCombo->addItems({
        "Package 1",
        "Package 2",
        "Package 3",
        "Package 4",
        "Package 5",
    });
    grid->addWidget(packageCombo, 2, 1);

    // Learned List Switch
    auto* learnedCheck = new QCheckBox(tr("Learned"));
    learnedCheck->setTristate(true);
    grid->addWidget(learnedCheck, 3, 1);

    // In Wishlist Switch
    auto* wishlistCheck = new QCheckBox(tr("In Wishlist"));
    wishlistCheck->setTristate(true);
    grid->addWidget(wishlistCheck, 4, 1);

    // Search Button
    auto* searchBtn = new QPushButton(tr("Search"));
    grid->addWidget(searchBtn, 5, 0, 1, 2);

    // Themes List
    auto* themesList = new ThemesListWidget;
    themesList->addItems({
        "Theme 1",
        "Theme 2",
        "Theme 3",
        "Theme 4",
        "Theme 5",
    });
    vbox->addWidget(themesList);
}
