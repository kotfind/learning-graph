#include "filesystem.h"

#include "../db/db.h"

#include <QDebug>

using namespace filesystem;

void list::exportAsTxt(const QString& filename) {
    theme::exportAsTxt(
        filename,
        db::list::getIds()
    );
}
