/* ========================================================================
   Author: Douglas B. Cuthbertson
   (C) Copyright 2015 by Douglas B. Cuthbertson. All Rights Reserved.
   ======================================================================== */
#pragma once


#include "platform.h"

BEGIN_EXTERN_C
// Basic Unit Test (BUT)

// A Handle for test data
DEFINE_HANDLE(TestData);

typedef
enum _but_test_result
{
    TEST_RESULT_PASSED,         // The test case was run and it returned successfully
    TEST_RESULT_FAILED,         // The test case was run and it returned a failure
    TEST_RESULT_EXC_FAILED,     // The main test passed, but an exception path failed
    TEST_RESULT_SETUP_FAILED,   // The setup function reported a failure
    TEST_RESULT_NOTRUN          // The test case has not run
} but_test_result;

typedef wch*    (*test_case_name_get)(void);
typedef s32     (*test_case_setup)(TestData);
typedef s32     (*test_case_run)(TestData);
typedef void    (*test_case_teardown)(TestData);

// Test case interface
typedef
struct _but_test_case_inf
{
    test_case_name_get  get;
    test_case_setup     setup;
    test_case_run       run;
    test_case_teardown  teardown;
} *but_test_case_inf;

// What kind of an internal struct do I need?
typedef
struct _but_test_case
{
    struct _but_test_case_inf   interface;
    TestData                    data;
} *but_test_case;

typedef const wch* (*test_suite_name_get)(void);
typedef size_t (*test_case_count)(void);
typedef but_test_case_inf (*test_case_get)(size_t index);
typedef but_test_case_inf (*test_case_first)(void);
typedef but_test_case_inf (*test_case_next)(but_test_case_inf);
typedef but_test_case_inf (*test_case_end)(void);

typedef
struct _but_test_suite_inf
{
    test_suite_name_get name;
    test_case_count     count;
    test_case_get       get;
    test_case_first     first;
    test_case_next      next;
    test_case_end       end;
} *test_suite_inf;

//! Move to but.c. This is the internal representation
typedef
struct _but_test_suite
{
    wch*                name;
    size_t              count;
    but_test_case_inf   testCases;
} *but_test_suite;

/**
 * A pointer to a function that will return the address of a ButTestSuite.
 * This is the only function that a shared/dynamic-link library (DLL) must
 * export so the test driver can retrieve the test suite.
 */
typedef but_test_suite (*but_test_suite_get)(void);

// getButTestSuite is the only function that a Basic Unit Test DLL must export.
but_test_suite_get test_suite_get(void);

END_EXTERN_C
