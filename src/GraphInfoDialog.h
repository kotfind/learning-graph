#pragma once

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class GraphInfoDialog : public QDialog {
    Q_OBJECT

    public:
        GraphInfoDialog(int graphId, QWidget* parent = nullptr);

        int getId() const { return graphId; }

    private:
        void ui();
        void load();

        int graphId;

        QLineEdit* nameEdit;
        QPushButton* cancelBtn;
        QPushButton* saveBtn;

    signals:
        void graphsUpdated();

    private slots:
        void save();
};
