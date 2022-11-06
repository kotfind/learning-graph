#include "DependencyDirectionDialog.h"

#include "DependencyDirection.h"

#include <QVBoxLayout>
#include <QLabel>

DependencyDirectionDialog::DependencyDirectionDialog(
    const QString& first,
    const QString& second,
    QWidget* parent)
    : QDialog(parent) {

    ui(first, second);

    connect(
        rightButton,
        &QPushButton::clicked,
        this,
        &DependencyDirectionDialog::onRightButtonClicked
    );

    connect(
        leftButton,
        &QPushButton::clicked,
        this,
        &DependencyDirectionDialog::onLeftButtonClicked
    );

    connect(
        cancelButton,
        &QPushButton::clicked,
        this,
        &DependencyDirectionDialog::onCancelButtonClicked
    );

    connect(
        skipCurrentButton,
        &QPushButton::clicked,
        this,
        &DependencyDirectionDialog::onSkipCurrentButtonClicked
    );

    connect(
        skipAllButton,
        &QPushButton::clicked,
        this,
        &DependencyDirectionDialog::onSkipAllButtonClicked
    );
}

void DependencyDirectionDialog::ui(const QString& first, const QString& second) {
    // Title
    setWindowTitle(tr("Dependency Direction Dialog"));

    // Layout
    auto* vbox = new QVBoxLayout;
    setLayout(vbox);

    // Label
    vbox->addWidget(new QLabel(tr("Select dependency directon.")));

    // Buttons
    rightButton = new QPushButton(
        tr("\"%1\" depends on \"%2\"").arg(first).arg(second)
    );
    vbox->addWidget(rightButton);

    leftButton = new QPushButton(
        tr("\"%1\" depends on \"%2\"").arg(second).arg(first)
    );
    vbox->addWidget(leftButton);

    cancelButton = new QPushButton(tr("Skip dependency"));
    vbox->addWidget(cancelButton);

    skipCurrentButton = new QPushButton(
        tr("Skip all dependences with \"%1\"").arg(first)
    );
    vbox->addWidget(skipCurrentButton);

    skipAllButton = new QPushButton(tr("Skip All"));
    vbox->addWidget(skipAllButton);
}

void DependencyDirectionDialog::onRightButtonClicked() {
    direction = RIGHT_DIRECTION;
    emit accept();
}

void DependencyDirectionDialog::onLeftButtonClicked() {
    direction = LEFT_DIRECTION;
    emit accept();
}

void DependencyDirectionDialog::onCancelButtonClicked() {
    direction = CANCEL_DIRECTION;
    emit accept();
}

void DependencyDirectionDialog::onSkipCurrentButtonClicked() {
    direction = SKIP_CURRENT_DIRECTIONS;
    emit accept();
}

void DependencyDirectionDialog::onSkipAllButtonClicked() {
    direction = SKIP_ALL_DIRECTIONS;
    emit accept();
}
