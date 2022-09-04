#pragma once

class QString;

namespace db {
    bool init();

    namespace theme {
        QString name(int id);
        QString packageName(int id);
        void read(
            int id,
            QString* name,
            int* packageId,
            QString* description,
            bool* inWishlist,
            bool* isLearned
        );

        // Inserts theme if id == -1; updates otherwise
        // Return value is id
        int write(
            int id,
            const QString& name,
            int packageId,
            const QString& description,
            bool inWishlist,
            bool isLearned
        );

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
