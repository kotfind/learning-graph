#pragma once

#include <QObject>

class GlobalSignalHandler : public QObject {
    Q_OBJECT

    public:
        GlobalSignalHandler(QObject* parent = nullptr);

        static GlobalSignalHandler* getInstance();

    private:
        static GlobalSignalHandler* instance;

    signals:
        void themesUpdated();
        void packagesUpdated();
        void graphsUpdated();
        void fontSet(const QFont& font); // GraphNodeItems won't change font otherwise
};
