#pragma once

#include "Package.h"

#include <QString>

struct Theme {
    int id;
    QString name;
    Package package;
    QString description;
    bool inWishlist;
    bool isLearned;
};
