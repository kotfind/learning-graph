#include "PackagesListWidget.h"

#include "../logics/WorkerCore.h"

#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>

PackagesListWidget::PackagesListWidget(QWidget* parent)
        : QListWidget(parent) {
    setSelectionMode(QListWidget::NoSelection);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &PackagesListWidget::customContextMenuRequested,
            this, &PackagesListWidget::showContextMenu);

    connect(this, &PackagesListWidget::listRequested,
            WorkerCore::getInstance(), &WorkerCore::getPackagesList);
    connect(WorkerCore::getInstance(), &WorkerCore::packagesChanged,
            WorkerCore::getInstance(), &WorkerCore::getPackagesList);
    connect(WorkerCore::getInstance(), &WorkerCore::packagesListGot,
            this, &PackagesListWidget::onListGot);
    connect(this, &PackagesListWidget::deletePackage,
            WorkerCore::getInstance(), &WorkerCore::deletePackage);
    connect(this, &PackagesListWidget::updatePackage,
            WorkerCore::getInstance(), &WorkerCore::updatePackage);
}

void PackagesListWidget::onListGot(const QVector<Package>& packages) {
    clear();
    for (const auto& package : packages) {
        auto* item = new QListWidgetItem(package.name);
        item->setData(Qt::UserRole, package.id);
        addItem(item);
    }
}

void PackagesListWidget::showContextMenu(const QPoint& pos) {
    auto* curr = currentItem();
    if (!curr) {
        return;
    }

    QMenu menu;

    menu.addAction(tr("Delete"), [=]() {
        if (QMessageBox::question(this, "Question",
                tr("Delete package \"%1\"?").arg(curr->text()))
                == QMessageBox::Yes) {
            emit deletePackage(curr->data(Qt::UserRole).toInt());
        }
    });

    menu.addAction(tr("Rename"), [=]() {
        bool ok;
        auto name = QInputDialog::getText(this, tr("Rename package"),
            tr("Package name:"), QLineEdit::Normal, curr->text(), &ok).trimmed();
        if (ok) {
            emit updatePackage(Package{
                curr->data(Qt::UserRole).toInt(),
                name
            });
        }
    });

    menu.exec(mapToGlobal(pos));
}
