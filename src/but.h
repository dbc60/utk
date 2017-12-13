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

typedef wch*    (*GetNameTestCase)(void);
typedef s32     (*SetupTestCase)(TestDataHandle);
typedef s32     (*RunTestCase)(TestDataHandle);
typedef void    (*TeardownTestCase)(TestDataHandle);

// Test case interface
typedef
struct ButTestCaseInf
{
    GetNameTestCase     get;
    SetupTestCase       setup;
    RunTestCase         run;
    TeardownTestCase    teardown;
} ButTestCaseInf, *PButTestCaseInf;

// What kind of an internal struct do I need?
typedef
struct ButTestCase
{
    ButTestCaseInf  interface;
    TestDataHandle  data;
} BetTestCase, *PButTestCase;

typedef const wch* (*GetNameTestSuite)(void);
typedef size_t (*CountTestCases)(void);
typedef PButTestCaseInf (*GetTestCase)(size_t index);
typedef PButTestCaseInf (*FirstTestCase)(void);
typedef PButTestCaseInf (*NextTestCase)(PButTestCaseInf);
typedef PButTestCaseInf (*EndTestCase)(void);

typedef
struct ButTestSuiteInf
{
    GetNameTestSuite    name;
    CountTestCases      count;
    GetTestCase         get;
    FirstTestCase       first;
    NextTestCase        next;
    EndTestCase         end;
} ButTestSuiteInf, *PButTestSuiteInf;

//! Move to but.c. This is the internal representation
typedef
struct ButTestSuite
{
    wch*            name;
    size_t          count;
    PButTestCaseInf testCases;
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
