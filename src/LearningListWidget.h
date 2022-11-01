#pragma once

#include "SmartListWidget.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class LearningListWidget : public QWidget {
    public:
        LearningListWidget(QWidget* parent = nullptr);

    private:
        void ui();

        QLabel* mainLabel;
        SmartListWidget* themesList;
        QPushButton* exportButton;
        QPushButton* closeButton;

    private slots:
        void openList(int themeId);
};
