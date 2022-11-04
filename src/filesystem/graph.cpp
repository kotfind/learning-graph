#include "filesystem.h"

#include "../GraphScene.h"
#include "../db/db.h"

#include <QMargins>
#include <QSvgGenerator>
#include <QFile>
#include <QDataStream>

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

void graph::exportAsGraph(const QString& filename, int id) {
    QFile file(filename);
    if (!file.open(QFile::WriteOnly)) {
        throw 0;
    }
    QDataStream out(&file);

    // Write graph name
    out << db::graph::name(id);

    // Write nodes
    auto nodes = db::graphNode::reads(id);
    out << (qint32)nodes.size();
    for (const auto& n : nodes) {
        auto t = db::theme::read(n.themeId);
        out << t.package.name
            << t.name
            << n.x
            << n.y;
    }
}

void graph::importFromGraph(const QString& filename) {
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        throw 0;
    }
    QDataStream in(&file);

    // Read graph name
    QString graphName;
    in >> graphName;
    if (!db::graph::unique(graphName)) {
        throw QObject::tr("Cannot import. Graph \"%1\" already exists.")
            .arg(graphName);
    }

    // Read nodes
    qint32 n;
    in >> n;
    QList<GraphNode> nodes(n);
    for (auto& n : nodes) {
        QString packageName;
        QString themeName;
        in >> packageName
           >> themeName
           >> n.x
           >> n.y;
        n.themeId = db::theme::find(packageName, themeName);
        if (n.themeId == -1) {
            throw QObject::tr("Cannot import. Theme \"%1 (%2)\" does not exist.")
                .arg(themeName)
                .arg(packageName);
        }
    }

    // Merge with db

    // Insert graph
    Graph g;
    g.id = -1;
    g.name = graphName;
    int graphId = db::graph::write(g);

    // Insert nodes
    for (auto& n : nodes) {
        n.graphId = graphId;
        db::graphNode::create(n);
    }
}
