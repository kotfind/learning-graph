#pragma once

#include "../types.h"

#include <QString>
#include <QList>

namespace db {
    bool init();

    namespace theme {
        // Returns name of theme with id id
        QString name(int id);

        // Returns name of package of theme with id id
        QString packageName(int id);

        // Returns Theme structure for theme with id id
        Theme read(int id);

        // Returns list of themes with current filters
        // Theme name is LIKE (in SQL terms) %name%
        // Theme packageId equals packageId
        //     or can be any if packageId == -1
        // Theme should be in wishlist if inWishlist == Qt::Checked,
        //     should not be in wishlist if inWishlist == Qt::Unchecked,
        //     can be or not be in wishlist if inWishlist == Qt::PartlyChecked
        // Theme should be learned if isLearned == Qt::Checked,
        //     should not be learned if isLearned == Qt::Unchecked,
        //     can be or not be learned if isLearned == Qt::PartlyChecked
        // XXX: No description selected
        QList<Theme> reads(
            const QString& name,
            int packageId,
            Qt::CheckState inWishlist,
            Qt::CheckState isLearned
        );

        // Returns list of themes which are not
        // presenred on graph with id excludeGraphId
        // XXX: No description selected
        QList<Theme> readsExceptGraph(int excludeGraphId);

        // Selects themes with ids ids
        // XXX: description is selected
        QList<Theme> readsByIds(const QList<int>& ids);

        // Returns list of themes so that
        //     theme with id themeId depends on themes from list
        // XXX: No description selected
        QList<Theme> readsDependencies(int themeId);

        // Inserts theme into db if id == -1,
        //    updates existant theme otherwise
        // Returns theme's id
        int write(const Theme&);

        // Deletes theme with id id
        void del(int id);

        // Exports themes with ids ids into file "filename" as txt
        void exportAsTxt(const QString& filename, const QList<int>& ids);

        // Returns true if theme with id id exists in db
        // Return false otherwise
        bool exists(int id);
    };

    namespace package {
        // Returns name of package with id id
        QString name(int id);

        // Returns quantity of themes in package with id id
        int count(int id);

        // Returns Package structure for package with id id
        Package read(int id);

        // Returns list of packages with
        // package name LIKE (in SQL terms) %name%
        QList<Package> reads(const QString& name);

        // Inserts package into db if id == -1,
        //    updates existant package otherwise
        // Returns package's id
        int write(const Package&);

        // Deletes package with id id
        void del(int id);

        // Returns ids of themes from packages with ids packageIds
        QList<int> getThemeIds(const QList<int>& packageIds);

        // Exports packages with ids ids into file "filename" as txt
        void exportAsTxt(const QString& filename, const QList<int>& ids);
    };

    namespace graph {
        // Returns name of graph with id id
        QString name(int id);

        // Returns quantity of themes in graph with id id
        int count(int id);

        // Returns Graph structure for graph with id id
        Graph read(int);

        // Returns list of graphs with
        // graph name LIKE (in SQL terms) %name%
        QList<Graph> reads(const QString& name);

        // Inserts graph into db if id == -1,
        //    updates existant graph otherwise
        // Returns graph's id
        int write(const Graph&);

        // Deletes graph with id id
        void del(int id);

        // Returns true if graph with id id exists in db
        // Return false otherwise
        bool exists(int id);
    };

    namespace graphNode {
        // Returns GraphNode structure for graphNode with id id
        GraphNode read(int id);

        // Return graphNodes presented on graph with id graphId
        QList<GraphNode> reads(int graphId);

        // Inserts graphNode n into db
        int create(const GraphNode& n);

        // Sets position of graphNode with id id to (x, y)
        void move(int id, double x, double y);

        // Deletes graphNode with id id
        void del(int id);
    };

    namespace themeEdge {
        // Returns themeEdges presented on graph with id graphId
        // If themeId != -1, than returns only edges
        //     than begin or end in theme with id themeId
        QList<ThemeEdge> reads(int graphId, int themeId);

        // Inserts edge
        //     from node with id beginNodeId
        //     to   node with id endNodeId
        // into db
        // Returns edge's is
        int create(int beginNodeId, int endNodeId);

        // Deletes edge with id edgeId
        void del(int edgeId);
    };
};
