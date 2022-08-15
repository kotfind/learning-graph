#pragma once

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

#define LOG_PREPARE(query, str) \
if (!(query).prepare(str)) { \
    qDebug() << "FAILED TO PREPARE QUERY at" \
        << __FILE__ << __LINE__ \
        << (query.lastError()); \
}

#define LOG_EXEC(query) \
if (!(query).exec()) { \
    qDebug() << "FAILED TO EXEC QUERY at" \
        << __FILE__ << __LINE__ \
        << (query.lastError()); \
}
