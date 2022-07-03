#pragma once

#include <QVector>
#include <QString>

struct Theme {
    int id;
    QString name;
    int packageId;
    QString description;
    bool inWishlist;
    bool isLearned;
    QVector<int> dependsOn;
};
