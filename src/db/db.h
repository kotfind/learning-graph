#pragma once

#include <QString>
#include <tuple>
#include <QList>

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

    struct ThemeForList {
        int id;
        QString name;
        QString packageName;
    };

    struct PackageForList {
        int id;
        QString name;
        int count;
    };

    namespace theme {
        QString name(int id);
        QString packageName(int id);
        Theme read(int id);
        QList<ThemeForList> readForList(
            const QString& name,
            int packageId,
            Qt::CheckState inWishlist,
            Qt::CheckState isLearned
        );

        // Inserts theme if id == -1; updates otherwise
        // Return value is id
        int write(const Theme&);

        void del(int id);
    };

    namespace package {
        QString name(int id);
        int count(int id);
        QList<PackageForList> readForList(const QString& name);

        // Inserts theme if id == -1; updates otherwise
        // Return value is id
        int write(
            int id,
            const QString& name
        );

        void del(int id);
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
