#pragma once

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class PackageInfoDialog : public QDialog {
    Q_OBJECT

    public:
        PackageInfoDialog(int packageId, QWidget* parent = nullptr);

    private:
        void ui();
        void load();

        int getId() const { return packageId; }

        int packageId;

        QLineEdit* nameEdit;
        QPushButton* cancelBtn;
        QPushButton* saveBtn;

    signals:
        void packagesUpdated();

    private slots:
        void save();
};
