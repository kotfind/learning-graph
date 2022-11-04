#pragma once

#include <QString>

struct Package {
    Package();

    qint32 id = -1;
    QString name = "";
    qint32 count = -1;
};

struct Theme {
    Theme();

    qint32 id = -1;
    QString name = "";
    Package package = Package();
    bool inWishlist = false;
    bool isLearned = false;
    QString description = "";
};

struct Graph {
    Graph();

    qint32 id = -1;
    QString name = "";
    qint32 count = -1;
};

struct GraphNode {
    GraphNode();

    qint32 id = -1;
    qint32 graphId = -1;
    qint32 themeId = -1;
    double x = 0;
    double y = 0;
};

struct ThemeEdge {
    ThemeEdge();

    qint32 id = -1;
    qint32 beginId = -1;
    qint32 endId = -1;
};
