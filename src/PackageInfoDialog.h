#pragma once

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class PackageInfoDialog : public QDialog {
    Q_OBJECT

    public:
        PackageInfoDialog(int packageId, QWidget* parent = nullptr);

        // Returns current package's id
        int getId() const { return packageId; }

    private:
        // Draws user interface
        void ui();

        // Loads data from db
        void load();

        int packageId;

        QLineEdit* nameEdit;
        QPushButton* cancelButton;
        QPushButton* saveButton;

    signals:
        void packagesUpdated();

    private slots:
        // Writes package to db
        void save();
};
