#pragma once

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class GraphInfoDialog : public QDialog {
    Q_OBJECT

    public:
        // Constructs GraphInfoDialog for graphs with id graphId
        GraphInfoDialog(int graphId, QWidget* parent = nullptr);

        // Returns graph's id
        int getId() const { return graphId; }

    private:
        // Draws user interface
        void ui();

        // Loads graph's data from db
        void load();

        int graphId;

        QLineEdit* nameEdit;
        QPushButton* cancelButton;
        QPushButton* saveButton;

    signals:
        void graphsUpdated();

    private slots:
        // Writes graphs to db
        void save();
};
