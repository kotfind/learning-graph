#pragma once

#include "../datatypes/Theme.h"
#include "../datatypes/ThemeRequest.h"

#include "../datatypes/Package.h"
#include "../datatypes/PackageRequest.h"

#include <QObject>
#include <QString>
#include <QVector>
#include <QSqlDatabase>

class WorkerCore : public QObject {
    Q_OBJECT

    public:
        static WorkerCore* getInstance();

    private:
        WorkerCore();

        void initDb();

        static WorkerCore* instance;

        const QString dbFilename = "learning_graph.db"; // XXX
        QSqlDatabase db;

    signals:
        void errorGot(const QString& error);

        void themesListGot(const QVector<Theme>& themes);
        void themeGot(const Theme& theme);

        void packagesListGot(const QVector<Package>& packages);
        void packageGot(const Package& packages);
        void packagesChanged();

    public slots:
        void getThemesList(
            const ThemeRequest& fields,
            const QString& themeNameKeyword,
            int packageId,
            Qt::CheckState inWishlist,
            Qt::CheckState isLearned);
        void getTheme(const ThemeRequest& fields, int themeId);

        void getPackagesList(
            const PackageRequest& fields);
        void getPackage(
            const PackageRequest& fields,
            int packageId);
        void createPackage(const QString& name);
};
