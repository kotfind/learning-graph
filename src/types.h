#pragma once

#include <QString>

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
