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

        // Selects themes with current options
        // XXX: No description selected
        QList<Theme> reads(
            const QString& name,
            int packageId,
            Qt::CheckState inWishlist,
            Qt::CheckState isLearned
        );

        // Selects themes that are not
        // presenred on current graph
        // XXX: No description selected
        QList<Theme> reads(int excludeGraph);

        // Selects themes with current ids
        // XXX: description is selected
        QList<Theme> reads(const QList<int>& ids);

        // Inserts theme if id == -1; updates otherwise
        // Return value is id
        int write(const Theme&);

        void del(int id);

        void exportAsTxt(const QString& filename, const QList<int>& ids);

        bool exists(int id);
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

        // returns ids of themes from multiple packages
        QList<int> getThemeIds(const QList<int>& packageIds);

        void exportAsTxt(const QString& filename, const QList<int>& ids);
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

        bool exists(int id);
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

        void del(int edgeId);
    };

    // Learning List
    namespace list {
        void build(int themeId);

        // Selects themes that
        // presented in list
        QList<Theme> reads();
    };
};
