#pragma once

#include <QDebug>

int __TEST_NUMBER__;
int __TESTS_PASSED__;
int __TESTS_FAILED__;

#define TEST_GROUP_BEGIN(name) \
    __TEST_NUMBER__ = 1; \
    __TESTS_PASSED__ = 0; \
    __TESTS_FAILED__ = 0; \
    qDebug().noquote() << \
        QString("---------- Testing %1 ----------").arg(name);

#define TEST_PASSED \
    qDebug().noquote() << \
        QString("Test %1: ok").arg(__TEST_NUMBER__);\
    ++__TEST_NUMBER__; \
    ++__TESTS_PASSED__;

#define TEST_FAILED \
    qDebug().noquote() << \
        QString("Test %1: FAILED").arg(__TEST_NUMBER__);\
    ++__TEST_NUMBER__; \
    ++__TESTS_FAILED__;

#define TEST_CHECK(v) \
    if (v) { \
        TEST_PASSED \
    } else { \
        TEST_FAILED \
    } \

#define TEST_COMPARE(a, b) \
    TEST_CHECK((a) == (b))

#define TEST_GROUP_END \
    qDebug().noquote() << \
        QString("---------- Done (Passed: %1, Failed: %2) ----------\n") \
            .arg(__TESTS_PASSED__) \
            .arg(__TESTS_FAILED__);
