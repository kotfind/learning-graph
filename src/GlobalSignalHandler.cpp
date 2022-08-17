#include "GlobalSignalHandler.h"

#include <QDebug>

GlobalSignalHandler* GlobalSignalHandler::instance = nullptr;

GlobalSignalHandler::GlobalSignalHandler(QObject* parent)
    : QObject(parent) {
}

GlobalSignalHandler* GlobalSignalHandler::getInstance() {
    if (!instance) {
        instance = new GlobalSignalHandler();
    }
    return instance;
}
