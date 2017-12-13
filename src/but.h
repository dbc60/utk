/* ========================================================================
   Author: Douglas B. Cuthbertson
   (C) Copyright 2015 by Douglas B. Cuthbertson. All Rights Reserved.
   ======================================================================== */
#pragma once


#include "platform.h"

BEGIN_EXTERN_C
// Basic Unit Test (BUT)

// Handles for Test Data and Test Context
DEFINE_HANDLE(TestDataHandle);
DEFINE_HANDLE(TestContextHandle);

typedef
enum ButTestResult
{
    TEST_RESULT_PASSED,         // The test case was run and it returned successfully
    TEST_RESULT_FAILED,         // The test case was run and it returned a failure
    TEST_RESULT_EXC_FAILED,     // The main test passed, but an exception path failed
    TEST_RESULT_SETUP_FAILED,   // The setup function reported a failure
    TEST_RESULT_NOTRUN          // The test case has not run
} ButTestResult;

typedef s32     (*setupType)(TestDataHandle);
typedef s32     (*runTestType)(TestDataHandle);
typedef void    (*teardownType)(TestDataHandle);

typedef
struct ButTestCase
{
    wch*            name;
    setupType       setup;
    runTestType     run;
    teardownType    teardown;
    TestDataHandle  testData;
} ButTestCase, *PButTestCase;

typedef
struct ButTestSuite
{
    wch*            name;
    size_t          count;
    PButTestCase    testCases;
} ButTestSuite, *PButTestSuite;

/**
 * A pointer to a function that will return the address of a ButTestSuite.
 * This is the only function that a shared/dynamic-link library (DLL) must
 * export so the test driver can retrieve the test suite.
 */
typedef PButTestSuite (*FPButTestSuite)(void);

// getButTestSuite is the only function that a Basic Unit Test DLL must export.
PButTestSuite getButTestSuite(void);

END_EXTERN_C
