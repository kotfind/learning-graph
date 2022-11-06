#pragma once

#include "DependencyDirection.h"

#include <QDialog>
#include <QPushButton>

class DependencyDirectionDialog : public QDialog {
    public:
        // Dialog asks dependency direction
        //     between first and second
        DependencyDirectionDialog(
            const QString& first,
            const QString& second,
            QWidget* parent = nullptr
        );

        DependencyDirection getDirection() { return direction; }

    private:
        // Draws user interface
        void ui(
            const QString& first,
            const QString& second
        );

        QPushButton* rightButton;
        QPushButton* leftButton;
        QPushButton* cancelButton;
        QPushButton* skipCurrentButton;
        QPushButton* skipAllButton;

        DependencyDirection direction = CANCEL_DIRECTION;

    private slots:
        // Sets direction to RIGHT_DIRECTION
        // Emits accept
        void onRightButtonClicked();

        // Sets direction to LEFT_DIRECTION
        // Emits accept
        void onLeftButtonClicked();

        // Sets direction to CANCEL_DIRECTION
        // Emits accept
        void onCancelButtonClicked();

        // Sets direction to SKIP_CURRENT_DIRECTIONS
        // Emits accept
        void onSkipCurrentButtonClicked();

        // Sets direction to SKIP_ALL_DIRECTIONS
        // Emits accept
        void onSkipAllButtonClicked();
};
