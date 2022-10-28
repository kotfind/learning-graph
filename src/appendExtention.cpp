#include "appendExtention.h"

void appendExtentionIfNot(QString& filename, const QString& extention) {
    if (!filename.endsWith(extention)) {
        filename += extention;
    }
}
