#include "filesystem.h"

#include "../db/db.h"

#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QHash>
#include <qhash.h>

using namespace filesystem;

void package::exportAsTxt(const QString& filename, const QList<int>& ids) {
    theme::exportAsTxt(
        filename,
        db::package::getThemeIds(ids)
    );
}

void package::exportAsPack(const QString& filename, const QList<int>& ids) {
    QFile file(filename);
    if (!file.open(QFile::WriteOnly)) {
        throw 0;
    }
    QDataStream out(&file);

    // Write packages
    auto packages = db::package::readsByIds(ids);
    out << (int)packages.size();
    for (const auto& p : packages) {
        out << (int)p.id
            << p.name;
    }

    // Write themes
    auto themeIds = db::package::getThemeIds(ids);
    auto themes = db::theme::readsByIds(themeIds);
    out << (int)themes.size();
    for (const auto& t : themes) {
        out << (int)t.id
            << t.name
            << (int)t.package.id
            << t.description
            << t.inWishlist
            << t.isLearned;
    }

    // Write edges
    auto edges = db::themeEdge::readsFromThemeIds(themeIds);
    out << (int)edges.size();
    for (const auto& e : edges) {
        out << (int)e.id
            << (int)e.beginId
            << (int)e.endId;
    }
}

void package::importFromPack(const QString &filename) {
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        throw 0;
    }
    QDataStream in(&file);

    // Read packages
    int n;
    in >> n;
    QList<Package> packages(n);
    for (auto& p : packages) {
        in >> p.id
           >> p.name;
    }

    // Read themes
    in >> n;
    QList<Theme> themes(n);
    for (auto& t : themes) {
        in >> t.id
           >> t.name
           >> t.package.id
           >> t.description
           >> t.inWishlist
           >> t.isLearned;
    }

    // Reads edges
    in >> n;
    QList<ThemeEdge> edges(n);
    for (auto& e : edges) {
        in >> e.id
           >> e.beginId
           >> e.endId;
    }

    // Todo merge with db
    for (const auto& p : packages) {
        if (!db::package::unique(p.name)) {
            throw QObject::tr("Cannot import. Package \"%1\" already exists.")
                .arg(p.name);
        }
    }

    // Insert packages
    QHash<int, int> newPackageId; // newPackageId[oldId] = newId

    for (auto& p : packages) {
        int oldId = p.id;
        p.id = -1;
        newPackageId[oldId] = p.id = db::package::write(p);
    }

    // Insert themes
    QHash<int, int> newThemeId; // newThemeId[oldId] = newId

    for (auto& t : themes) {
        int oldId = t.id;
        t.id = -1;
        t.package.id = newPackageId[t.package.id];
        newThemeId[oldId] = t.id = db::theme::write(t);
    }

    // Insert edges
    for (const auto& e : edges) {
        db::themeEdge::createByThemes(
            newThemeId[e.beginId],
            newThemeId[e.endId]
        );
    }
}
