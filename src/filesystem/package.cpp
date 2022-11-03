#include "filesystem.h"

#include "../db/db.h"

using namespace filesystem;

void package::exportAsTxt(const QString& filename, const QList<int>& ids) {
    theme::exportAsTxt(
        filename,
        db::package::getThemeIds(ids)
    );
}
