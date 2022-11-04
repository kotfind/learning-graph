#pragma once

#include "SmartListWidget.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class LearningListWidget : public QWidget {
    Q_OBJECT

    public:
        LearningListWidget(QWidget* parent = nullptr);

    private:
        void ui();

        QLabel* mainLabel;
        SmartListWidget* themesList;
        QPushButton* exportButton;
        QPushButton* closeButton;

    private slots:
        void open(int themeId);
        void close();
};
