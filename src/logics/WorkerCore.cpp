#include "WorkerCore.h"

WorkerCore* WorkerCore::instance = nullptr;

WorkerCore::WorkerCore()
        : QObject() {
}

WorkerCore* WorkerCore::getInstance() {
    if (instance == nullptr) {
        instance = new WorkerCore;
    }

    return instance;
}

// Slots
void WorkerCore::getThemesList(
    const ThemeRequest& fields,
    const QString& themeNameKeyword,
    int packageId,
    Qt::CheckState inWishlist,
    Qt::CheckState isLearned) {
    QVector<Theme> themes = {
        Theme{1, "Theme 1", 1, "Theme 1 Description", false, false, {}},
        Theme{2, "Theme 2", 2, "Theme 2 Description", false, true, {}},
        Theme{3, "Theme 3", 3, "Theme 3 Description", true, false, {}},
        Theme{4, "Theme 4", 4, "Theme 4 Description", true, true, {}},
    };
    emit themesListGot(themes);
}

void WorkerCore::getTheme(const ThemeRequest& fields, int themeId) {
    emit themeGot(Theme{
        themeId,
        QString("Theme %1").arg(themeId),
        themeId,
        QString("Theme %1 Description").arg(themeId),
        false, false, {}});
}

void WorkerCore::getPackagesList(
    const PackageRequest& fields,
    const QString& packageNameKeyword) {
    QVector<Package> packages = {
        Package{1, "Package 1", {}},
        Package{2, "Package 2", {}},
        Package{3, "Package 3", {}},
        Package{4, "Package 4", {}},
    };
    emit packagesListGot(packages);
}
