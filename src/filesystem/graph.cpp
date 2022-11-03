#include "filesystem.h"

#include "../GraphScene.h"
#include "../db/db.h"

#include <QMargins>
#include <QSvgGenerator>

using namespace filesystem;

const QMargins exportMargins = {10, 10, 10, 10};

void graph::exportAsSvg(const QString& filename, GraphScene* graphScene) {
    graphScene->clearSelection();

    const auto rect = graphScene->itemsBoundingRect() + exportMargins;

    QSvgGenerator svgGen;
    svgGen.setFileName(filename);
    svgGen.setSize(rect.size().toSize());
    svgGen.setViewBox(rect);
    svgGen.setTitle(db::graph::name(graphScene->getGraphId()));

    QPainter painter(&svgGen);
    graphScene->render(&painter, rect, rect);
}

void graph::exportAsPng(const QString& filename, GraphScene* graphScene) {
    graphScene->clearSelection();

    const auto rect = graphScene->itemsBoundingRect() + exportMargins;

    QImage img(rect.size().toSize(), QImage::Format_ARGB32);
    img.fill(Qt::transparent);

    QPainter painter(&img);
    graphScene->render(&painter, QRectF(), rect);

    img.save(filename);
}

void graph::exportAsJpg(const QString& filename, GraphScene* graphScene) {
    graphScene->clearSelection();

    const auto rect = graphScene->itemsBoundingRect() + exportMargins;

    QImage img(rect.size().toSize(), QImage::Format_RGB32);
    img.fill(Qt::white);

    QPainter painter(&img);
    graphScene->render(&painter, QRectF(), rect);

    img.save(filename);
}
