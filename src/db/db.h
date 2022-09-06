#pragma once

#include <QString>

namespace db {
    bool init();

    struct Theme {
        int id;
        QString name;
        int packageId;
        bool inWishlist;
        bool isLearned;
        QString description;
    };

    namespace theme {
        QString name(int id);
        QString packageName(int id);
        Theme read(int id);

        // Inserts theme if id == -1; updates otherwise
        // Return value is id
        int write(const Theme&);

        void del(int id);
    };

    namespace package {
        QString name(int id);
        int count(int id);

        void del(int id);

        // Inserts theme if id == -1; updates otherwise
        // Return value is id
        int write(
            int id,
            const QString& name
        );
    };

    namespace graph {
        QString name(int id);
        int count(int id);

        void del(int id);

        // Inserts theme if id == -1; updates otherwise
        // Return value is id
        int write(
            int id,
            const QString& name
        );
    };
};
