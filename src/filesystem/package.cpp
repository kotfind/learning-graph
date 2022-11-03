#include "filesystem.h"

#include "../db/db.h"

#include <QFile>
#include <QDataStream>
#include <QDebug>

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
    out << (qint32)packages.size();
    for (const auto& p : packages) {
        out << (qint32)p.id
            << p.name;
    }

    // Write themes
    auto themes = db::theme::readsByIds(db::package::getThemeIds(ids));
    out << (qint32)themes.size();
    for (const auto& t : themes) {
        out << (qint32)t.id
            << t.name
            << (qint32)t.package.id
            << t.description
            << t.inWishlist
            << t.isLearned;
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
    qDebug() << n << "packages";
    for (auto& p : packages) {
        in >> p.id
           >> p.name;
        qDebug()
            << p.id
            << p.name;
    }

    // Read themes
    in >> n;
    QList<Theme> themes(n);
    qDebug() << n << "themes";
    for (auto& t : themes) {
        in >> t.id
           >> t.name
           >> t.package.id
           >> t.description
           >> t.inWishlist
           >> t.isLearned;
        qDebug()
            << t.id
            << t.name
            << t.package.id
            << t.description
            << t.inWishlist
            << t.isLearned;
    }

    // Todo merge with db
}
