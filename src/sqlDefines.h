#pragma once

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

#define LOG_FAILED_QUERY(query) \
    qDebug() << QString("QUERY FAILED at %1(%2)")\
        .arg(__FILE__) \
        .arg(__LINE__) \
        << (query).lastError();

#define R_PREPARE(query, str, r) \
if (!(query).prepare(str)) { \
    qDebug() << QString("PREPARE FAILED at %1(%2)")\
        .arg(__FILE__) \
        .arg(__LINE__) \
        << (query).lastError(); \
    return r; \
} \
(query).setForwardOnly(true);

#define R_PREPARE_NEW(query, str, r) \
QSqlQuery query; \
R_PREPARE(query, str, r)


#define R_EXEC(query, r) \
if (!(query).exec()) { \
    qDebug() << QString("EXEC FAILED at %1(%2)")\
        .arg(__FILE__) \
        .arg(__LINE__) \
        << (query).lastError(); \
    return r; \
}

#define SQLITE_CONSTRAINT_UNIQUE 2067
#define SQLITE_CONSTRAINT_NOTNULL 1299
#define SQLITE_CONSTRAINT_CHECK 275

#define ERR_CODE(query) (query).lastError().nativeErrorCode().toInt()

#define PREPARE(query, str) R_PREPARE(query, str,)
#define PREPARE_NEW(query, str) R_PREPARE_NEW(query, str,)
#define EXEC(query) R_EXEC(query,)
