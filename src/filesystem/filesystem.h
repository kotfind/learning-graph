#pragma once

#include "../types.h"

#include <QString>
#include <QList>

class GraphScene;

namespace filesystem {
    namespace theme {
        // Exports themes with ids ids into file "filename" as .txt
        void exportAsTxt(const QString& filename, const QList<int>& ids);
    };

    namespace package {
        // Exports packages with ids ids into file "filename" as .txt
        void exportAsTxt(const QString& filename, const QList<int>& ids);

        // Exports packages with ids ids into file "filename" as .pack
        void exportAsPack(const QString& filename, const QList<int>& ids);

        // Imports packages from .pack file "filename"
        void importFromPack(const QString& filename);
    };

    namespace graph {
        // Exports graph from scene graphScene as .jpg into file "filename"
        void exportAsJpg(const QString& filename, GraphScene* graphScene);

        // Exports graph from scene graphScene as .png into file "filename"
        void exportAsPng(const QString& filename, GraphScene* graphScene);

        // Exports graph from scene graphScene as .svg into file "filename"
        void exportAsSvg(const QString& filename, GraphScene* graphScene);

        // Exports graph with id id to file "filename" as .graph
        void exportAsGraph(const QString& filename, int id);

        // Imports graph from .graph file "filename"
        void importFromGraph(const QString& filename);
    };
};
