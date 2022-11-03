#include "filesystem.h"

#include "../db/db.h"

#include <QFile>
#include <QObject>
#include <QTextStream>

using namespace filesystem;

void theme::exportAsTxt(const QString& filename, const QList<int>& ids) {
    QFile file(filename);
    if (!file.open(QFile::WriteOnly)) {
        throw 0;
    }
    QTextStream out(&file);

    bool first = true;
    auto themes = db::theme::readsByIds(ids);
    for (const auto& theme : themes) {
        if (!first) {
            out << "\n";
        }
        first = false;

        out << QObject::tr("NAME: ") << theme.name << '\n';
        out << QObject::tr("PACKAGE: ") << theme.package.name << '\n';
        out << QObject::tr("LEARNED: ") << (theme.isLearned ? QObject::tr("Yes") : QObject::tr("No")) << '\n';
        out << QObject::tr("IN WISHLIST: ") << (theme.inWishlist ? QObject::tr("Yes") : QObject::tr("No")) << '\n';
        if (!theme.description.isEmpty()) {
            out << QObject::tr("DESCRIPTION:\n") << theme.description << '\n';
        }
        auto dependsOn = db::theme::readsDependencies(theme.id);
        if (!dependsOn.isEmpty()) {
            out << QObject::tr("DEPENDS ON:\n");
            for (const auto& t : dependsOn) {
                out << QObject::tr("+ %1 (%2)\n")
                    .arg(t.name)
                    .arg(t.package.name);
            }
        }
    }
}
