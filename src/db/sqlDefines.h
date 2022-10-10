#pragma once

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

#define LOG_FAILED_QUERY(query) \
    qDebug() << QString("QUERY FAILED at %1(%2)")\
        .arg(__FILE__) \
        .arg(__LINE__) \
        << (query).lastError();

#define PREPARE(query, str) \
if (!(query).prepare(str)) { \
    qDebug() << QString("PREPARE FAILED at %1(%2)")\
        .arg(__FILE__) \
        .arg(__LINE__) \
        << (query).lastError(); \
    throw 0; \
} \
(query).setForwardOnly(true);

#define PREPARE_NEW(query, str) \
QSqlQuery query; \
PREPARE(query, str)


#define EXEC(query) \
if (!(query).exec()) { \
    qDebug() << QString("EXEC FAILED at %1(%2)")\
        .arg(__FILE__) \
        .arg(__LINE__) \
        << (query).lastError(); \
    throw 0; \
}

#define SQLITE_CONSTRAINT_UNIQUE 2067
#define SQLITE_CONSTRAINT_NOTNULL 1299
#define SQLITE_CONSTRAINT_CHECK 275

#define ERR_CODE(query) (query).lastError().nativeErrorCode().toInt()
