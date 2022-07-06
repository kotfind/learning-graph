#pragma once

#include <QMainWindow>
#include <QString>

class GraphicsCore : public QMainWindow {
    public:
        GraphicsCore(QWidget* parent = nullptr);

    private slots:
        void onErrorGot(const QString& error);
};
