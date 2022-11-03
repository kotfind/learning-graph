#pragma once

#include <QString>

struct Package {
    Package();

    qint32 id;
    QString name;
    qint32 count;
};

struct Theme {
    Theme();

    qint32 id;
    QString name;
    Package package;
    bool inWishlist;
    bool isLearned;
    QString description;
};

struct Graph {
    Graph();

    qint32 id;
    QString name;
    qint32 count;
};

struct GraphNode {
    GraphNode();

    qint32 id;
    qint32 graphId;
    qint32 themeId;
    double x;
    double y;
};

struct ThemeEdge {
    ThemeEdge();

    qint32 id;
    qint32 beginId;
    qint32 endId;
};
