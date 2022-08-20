#pragma once

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

#define LOG_FAILED_QUERY(query) \
    qDebug() << QString("QUERY FAILED at %1(%2)")\
        .arg(__FILE__) \
        .arg(__LINE__) \
        << (query).lastError();

#define LOG_PREPARE(query, str) \
if (!(query).prepare(str)) { \
    qDebug() << QString("PREPARE FAILED at %1(%2)")\
        .arg(__FILE__) \
        .arg(__LINE__) \
        << (query).lastError(); \
} \
(query).setForwardOnly(true);

#define LOG_EXEC(query) \
if (!(query).exec()) { \
    qDebug() << QString("EXEC FAILED at %1(%2)")\
        .arg(__FILE__) \
        .arg(__LINE__) \
        << (query).lastError(); \
}

#define SQLITE_CONSTRAINT_UNIQUE 2067
#define SQLITE_CONSTRAINT_NOTNULL 1299

#define ERR_CODE(query) (query).lastError().nativeErrorCode().toInt()
