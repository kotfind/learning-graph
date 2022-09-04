class QString;

namespace db {
    bool init();

    namespace theme {
        QString name(int id);
        QString packageName(int id);
    };

    namespace package {
        QString name(int id);
        int count(int id);
    };

    namespace graph {
        QString name(int id);
        int count(int id);
    };
};
