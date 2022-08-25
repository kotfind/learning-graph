#include "ThemeTab.h"

#include "ThemeInfoDialog.h"
#include "sqlDefines.h"
#include "GlobalSignalHandler.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>

ThemeTab::ThemeTab(QWidget* parent)
        : QWidget(parent) {
    ui();

    connect(
        createBtn,
        &QPushButton::clicked,
        [this]() {
            (new ThemeInfoDialog(-1, this))->exec();
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

    connect(
        GlobalSignalHandler::getInstance(),
        &GlobalSignalHandler::packagesUpdated,
        this,
        &ThemeTab::update
    );

    connect(
        autoUpdateCheck,
        &QCheckBox::stateChanged,
        [this](int state) {
            setAutoUpdate(state == Qt::Checked);
        }
    );

    autoUpdateCheck->setChecked(true);
    update();
}

void ThemeTab::keyPressEvent(QKeyEvent* e) {
    switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            emit update();
            break;
    }
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

    // Search
    searchBtn = new QPushButton(tr("Search"));
    grid->addWidget(searchBtn, 5, 1);

    autoUpdateCheck = new QCheckBox(tr("Auto update"));
    grid->addWidget(autoUpdateCheck, 5, 0);

    // Themes List
    themesList = new ThemeListWidget;
    vbox->addWidget(themesList);
}

void ThemeTab::update() {
    QString queryString = " \
        SELECT id, name, (\
            SELECT name \
            FROM packages \
            WHERE packageId = id \
        ) \
        FROM themes \
        WHERE {whereSection} \
        GROUP BY ( \
            SELECT name \
            FROM packages \
            WHERE id = packageId \
        ), name \
    ";
    QVector<QVariant> params;

    QString whereSection = "TRUE";
    auto themeName = nameEdit->text().trimmed();
    if (themeName.size()) {
        whereSection += " AND themes.name LIKE '%' || ? || '%'";
        params.append(themeName);
    }

    auto packageId = packageCombo->currentData().toInt();
    if (packageId != -1) {
        whereSection += " AND packageId = ?";
        params.append(packageId);
    }

    auto inWishlist = wishlistCheck->checkState();
    if (inWishlist != Qt::PartiallyChecked) {
        whereSection += QString(" AND inWishlist = ?");
        params.append(inWishlist == Qt::Checked);
    }

    auto isLearned = learnedCheck->checkState();
    if (isLearned != Qt::PartiallyChecked) {
        whereSection += QString(" AND isLearned = ?");
        params.append(isLearned == Qt::Checked);
    }

    queryString.replace("{whereSection}", whereSection);

    PREPARE_NEW(query, queryString);
    for (const auto& param : params) {
        query.addBindValue(param);
    }
    EXEC(query)

    themesList->clear();
    while (query.next()) {
        auto name = QString("%1 (%2)")
            .arg(query.value(1).toString())
            .arg(query.value(2).toString());
        auto* item = new QListWidgetItem(name);
        item->setData(Qt::UserRole, query.value(0));
        themesList->addItem(item);
    }
}

void ThemeTab::setAutoUpdate(bool state) {
    if (state)  {
        searchBtn->setDisabled(true);

        connect(
            nameEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeTab::update
        );

        connect(
            packageCombo,
            &PackageComboBox::currentIndexChanged,
            this,
            &ThemeTab::update
        );

        connect(
            wishlistCheck,
            &QCheckBox::stateChanged,
            this,
            &ThemeTab::update
        );

        connect(
            learnedCheck,
            &QCheckBox::stateChanged,
            this,
            &ThemeTab::update
        );

        update();
    } else {
        searchBtn->setDisabled(false);

        disconnect(
            nameEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeTab::update
        );

        disconnect(
            packageCombo,
            &PackageComboBox::currentIndexChanged,
            this,
            &ThemeTab::update
        );

        disconnect(
            wishlistCheck,
            &QCheckBox::stateChanged,
            this,
            &ThemeTab::update
        );

        disconnect(
            learnedCheck,
            &QCheckBox::stateChanged,
            this,
            &ThemeTab::update
        );
    }
}
