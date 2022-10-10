#pragma once

#include <QString>
#include <QList>

namespace db {
    bool init();

    struct Package {
        Package();

        int id;
        QString name;
        int count;
    };

    struct Theme {
        Theme();

        int id;
        QString name;
        Package package;
        bool inWishlist;
        bool isLearned;
        QString description;
    };

    struct Graph {
        Graph();

        int id;
        QString name;
        int count;
    };

    struct GraphNode {
        GraphNode();

        int id;
        int graphId;
        int themeId;
        double x;
        double y;
    };

    struct ThemeEdge {
        ThemeEdge();

        int id;
        int beginId;
        int endId;
    };

    namespace theme {
        QString name(int id);
        QString packageName(int id);
        Theme read(int id);
        QList<Theme> reads(
            const QString& name,
            int packageId,
            Qt::CheckState inWishlist,
            Qt::CheckState isLearned,
            bool includeDescription = false
        );

        // Inserts theme if id == -1; updates otherwise
        // Return value is id
        int write(const Theme&);

        void del(int id);
    };

    namespace package {
        QString name(int id);
        int count(int id);
        Package read(int);
        QList<Package> reads(const QString& name);

        // Inserts theme if id == -1; updates otherwise
        // Return value is id
        int write(const Package&);

        void del(int id);
    };

    namespace graph {
        QString name(int id);
        int count(int id);
        Graph read(int);
        QList<Graph> reads(const QString& name);

        // Inserts theme if id == -1; updates otherwise
        // Return value is id
        int write(const Graph&);

        void del(int id);
    };

    namespace graphNode {
        GraphNode read(int id);
        QList<GraphNode> reads(int graphId);

        int create(const GraphNode&);

        void move(int id, double x, double y);

        void del(int id);
    };

    namespace themeEdge {
        // Set themeId = -1 for any themeId
        QList<ThemeEdge> reads(int graphId, int themeId);

        int create(int beginNodeId, int endNodeId);
    };
};
