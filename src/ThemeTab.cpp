#include "ThemeTab.h"

#include "ThemeInfoWindow.h"
#include "sqlDefines.h"
#include "GlobalSignalHandler.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QSqlQuery>

ThemeTab::ThemeTab(QWidget* parent)
        : QWidget(parent) {
    ui();

    connect(
        createBtn,
        &QPushButton::clicked,
        []() {
            (new ThemeInfoWindow(-1))->show();
        }
    );

    connect(
        searchBtn,
        &QPushButton::clicked,
        this,
        &ThemeTab::update
    );

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::themesUpdated,
        this,
        &ThemeTab::update
    );

    update();
}

void ThemeTab::ui() {
    // Layout
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    // Create Button
    createBtn = new QPushButton(tr("New theme"));
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

    nameEdit = new QLineEdit;
    grid->addWidget(nameEdit, 1, 1);

    // Package
    auto* packageLbl = new QLabel(tr("Package:"));
    grid->addWidget(packageLbl, 2, 0, Qt::AlignRight);

    packageCombo = new PackageComboBox;
    packageCombo->setAny(true);
    packageCombo->setCurrent(-1);
    grid->addWidget(packageCombo, 2, 1);

    // In Wishlist Switch
    wishlistCheck = new QCheckBox(tr("In Wishlist"));
    wishlistCheck->setTristate(true);
    wishlistCheck->setCheckState(Qt::PartiallyChecked);
    grid->addWidget(wishlistCheck, 4, 1);

    // Learned List Switch
    learnedCheck = new QCheckBox(tr("Learned"));
    learnedCheck->setTristate(true);
    grid->addWidget(learnedCheck, 3, 1);

    // Search Button
    searchBtn = new QPushButton(tr("Search"));
    grid->addWidget(searchBtn, 5, 0, 1, 2);

    // Themes List
    themesList = new ThemeListWidget;
    vbox->addWidget(themesList);
}

void ThemeTab::update() {
    QString queryString = " \
       SELECT id, name \
       FROM themes \
       WHERE TRUE \
    ";
    QVector<QVariant> params;

    auto themeName = nameEdit->text().trimmed();
    if (themeName.size()) {
        queryString += " AND name LIKE ('%' + ? + '%')";
        params.append(themeName);
    }

    auto packageId = packageCombo->currentData().toInt();
    if (packageId != -1) {
        queryString += " AND package_id = ?";
        params.append(packageId);
    }

    auto inWishlist = wishlistCheck->checkState();
    if (inWishlist != Qt::PartiallyChecked) {
        queryString += QString(" AND in_wishlist = ?");
        params.append(inWishlist == Qt::Checked);
    }

    auto isLearned = learnedCheck->checkState();
    if (isLearned != Qt::PartiallyChecked) {
        queryString += QString(" AND is_learned = ?");
        params.append(isLearned == Qt::Checked);
    }

    QSqlQuery query;
    LOG_PREPARE(query, queryString);
    for (const auto& param : params) {
        query.addBindValue(param);
    }
    LOG_EXEC(query)

    themesList->clear();
    while (query.next()) {
        auto* item = new QListWidgetItem(query.value(0).toString());
        item->setData(Qt::UserRole, query.value(1));
        themesList->addItem(item);
    }
}
