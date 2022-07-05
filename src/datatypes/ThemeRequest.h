#pragma once

#include "PackageRequest.h"

struct ThemeRequest {
    bool name;
    PackageRequest package;
    bool description;
    bool inWishlist;
    bool isLearned;
    bool dependsOn;
};
