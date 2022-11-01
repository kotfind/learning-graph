#pragma once

#include <QObject>

// Class works as tunnel for sending signals
// when it's hard to send them directly
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
