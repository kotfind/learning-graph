#pragma once

#include "Package.h"

#include <QVector>
#include <QString>

struct Theme {
    int id;
    QString name;
    Package package;
    QString description;
    bool inWishlist;
    bool isLearned;
    QVector<int> dependsOn;
};
