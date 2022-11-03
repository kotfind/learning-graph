#pragma once

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

// Prints debug information about failed query query
#define LOG_FAILED_QUERY(query) \
    qDebug().noquote() << QString("QUERY FAILED at %1(%2)")\
        .arg(__FILE__) \
        .arg(__LINE__) \
        << (query).lastError();

// Prepares query query from code str
// Prints debug info if failed
#define PREPARE(query, str) \
if (!(query).prepare(str)) { \
    qDebug().noquote() << QString("PREPARE FAILED at %1(%2)")\
        .arg(__FILE__) \
        .arg(__LINE__) \
        << (query).lastError(); \
    throw 0; \
} \
(query).setForwardOnly(true);

// Creates and prepares query query from code str
// Prints debug info if failed
#define PREPARE_NEW(query, str) \
QSqlQuery query; \
PREPARE(query, str)

// Executes query query
// Prints debug info if failed
#define EXEC(query) \
if (!(query).exec()) { \
    qDebug().noquote() << QString("EXEC FAILED at %1(%2)")\
        .arg(__FILE__) \
        .arg(__LINE__) \
        << (query).lastError(); \
    throw 0; \
}

// SQL error codes
#define SQLITE_CONSTRAINT_UNIQUE 2067
#define SQLITE_CONSTRAINT_NOTNULL 1299
#define SQLITE_CONSTRAINT_CHECK 275

// Returns error code of query query
#define ERR_CODE(query) (query).lastError().nativeErrorCode().toInt()
