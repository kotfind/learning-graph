#pragma once

#include "Theme.h"
#include "ThemeRequest.h"

#include "Package.h"
#include "PackageRequest.h"

#include <QObject>
#include <QString>
#include <QVector>

class WorkerCore : public QObject {
    Q_OBJECT

    public:
        static WorkerCore* getInstance();

    private:
        WorkerCore();

        static WorkerCore* instance;

    signals:
        void themesListGot(const QVector<Theme>& themes);
        void themeGot(const Theme& theme);
        void packagesListGot(const QVector<Package>& packages);

    public slots:
        void getThemesList(
            const ThemeRequest& fields,
            const QString& themeNameKeyword,
            int packageId,
            Qt::CheckState inWishlist,
            Qt::CheckState isLearned);
        void getTheme(const ThemeRequest& fields, int themeId);

        void getPackagesList(
            const PackageRequest& fields,
            const QString& packageNameKeyword);
};
