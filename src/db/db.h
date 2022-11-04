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
        // XXX: Selects id, name and package only
        QList<Theme> reads(
            const QString& name,
            int packageId,
            Qt::CheckState inWishlist,
            Qt::CheckState isLearned
        );

        // Returns list of themes which are not
        // presenred on graph with id excludeGraphId
        // XXX: Selects id, name and package only
        QList<Theme> readsExceptGraph(int excludeGraphId);

        // Returns list of themes with ids ids
        // XXX: If full == false,
        //    than selects id, name and package only
        // Selects everything otherwise
        QList<Theme> readsByIds(const QList<int>& ids, bool full);

        // Returns list of themes so that
        //     theme with id themeId depends on themes from list
        // XXX: Selects id, name and package only
        QList<Theme> readsDependencies(int themeId);

        // Inserts theme into db if id == -1,
        //    updates existant theme otherwise
        // Returns theme's id
        int write(const Theme&);

        // Deletes theme with id id
        void del(int id);

        // Returns true if theme with id id exists in db
        // Return false otherwise
        bool exists(int id);

        // Returns id of theme with name themeName
        //     and package name packageName
        // Returns -1 if not found
        int find(const QString& packageName, const QString& themeName);
    };

    namespace package {
        // Returns name of package with id id
        QString name(int id);

        // Returns quantity of themes in package with id id
        int count(int id);

        // Returns Package structure for package with id id
        Package read(int id);

        // Selects packages with ids ids
        QList<Package> readsByIds(const QList<int>& ids);

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

        // Returns true if name is unique
        bool unique(const QString& name);
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

        // Returns true if name is unique
        bool unique(const QString& name);
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

        // Returns themeId for node with id id
        int themeId(int id);
    };

    namespace themeEdge {
        // Returns themeEdges presented on graph with id graphId
        // If themeId != -1, than returns only edges
        //     than begin or end in theme with id themeId
        QList<ThemeEdge> readsFromGraph(int graphId, int themeId);

        // Returns themeEdges in which both end theme's ids are in ids
        QList<ThemeEdge> readsFromThemeIds(const QList<int>& ids);

        // Inserts edge
        //     from theme with id beginId
        //     to   theme with id endId
        // into db
        // Returns edge's is
        // XXX: won't check type of exception
        int createByThemes(int beginId, int endId);

        // Inserts edge
        //     from node with id beginNodeId
        //     to   node with id endNodeId
        // into db
        // Returns edge's is
        int createByNodes(int beginNodeId, int endNodeId);

        // Deletes edge with id edgeId
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
