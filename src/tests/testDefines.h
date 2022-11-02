#pragma once

#include <QDebug>
#include <QString>

int __TEST_NUMBER__;
int __TESTS_PASSED__;
int __TESTS_FAILED__;
QString __TEST_GROUP_NAME__;

#define TEST_GROUP_BEGIN(name) \
    __TEST_GROUP_NAME__ = (name); \
    __TEST_NUMBER__ = 1; \
    __TESTS_PASSED__ = 0; \
    __TESTS_FAILED__ = 0; \
    qDebug().noquote() << \
        QString("---------- Testing %1 ----------").arg(name);

#define TEST_PASSED \
    qDebug().noquote() << \
        QString("Test %1: pass").arg(__TEST_NUMBER__);\
    ++__TEST_NUMBER__; \
    ++__TESTS_PASSED__;

#define TEST_FAILED \
    qDebug().noquote() << \
        QString("Test %1: FAILED").arg(__TEST_NUMBER__);\
    ++__TEST_NUMBER__; \
    ++__TESTS_FAILED__;

#define TEST_CHECK(v) \
    try { \
        if (v) { \
            TEST_PASSED \
        } else { \
            TEST_FAILED \
        } \
    } catch (...) { \
        TEST_FAILED \
    }

#define TEST_COMPARE(a, b) \
    TEST_CHECK((a) == (b))

#define TEST_GROUP_END \
    qDebug().noquote() << \
        QString("Tested %1: (Passed: %2, Failed: %3)\n") \
            .arg(__TEST_GROUP_NAME__) \
            .arg(__TESTS_PASSED__) \
            .arg(__TESTS_FAILED__);
