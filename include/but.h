/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief TBD
 */

#ifndef BUT_H_INCLUDED
#define BUT_H_INCLUDED

#include "platform.h"

BEGIN_EXTERN_C
// Basic Unit Test (BUT)

// Forward declarations of "opaque" types
typedef struct but_context but_context;
typedef struct but_test_case but_test_case;
typedef struct but_test_suite but_test_suite;


enum but_test_results {
    BTR_PASSED,         // The test case was run and it returned successfully
    BTR_FAILED,         // The test case was run and it returned a failure
    BTR_FAILED_EXC,     // The main test passed, but an exception path failed
    BTR_FAILED_SETUP,   // The setup function reported a failure
    BTR_NOT_RUN,        // The test case has not run
    BTR_CONTEXT_INVALID // Something is wrong with the test context
};
typedef enum but_test_results but_test_result;

enum but_results {
    BUT_FAIL,
    BUT_SUCCESS
};
typedef enum but_results but_result;

typedef but_result  (but_method_setup)(void *data);
typedef but_result  (but_method_run)(void *data);
typedef void        (but_method_teardown)(void *data);

// Test case interface
struct but_test_case
{
    const ch8           *name;
    but_method_setup    *setup;
    but_method_run      *run;
    but_method_teardown *teardown;
    void                *test_data;
};
typedef struct but_test_case but_test_case;


/**
 * return the test case at index, or null if the index is not in the range of
 * zero to (count - 1).
 */
//typedef struct but_test_case* (*but_method_get)(size_t index);

struct but_test_suite
{
    const ch8          *name;
    size_t              count;
    but_test_case     **test_cases;
};
typedef struct but_test_suite but_test_suite;


/**
 * A pointer to a function that will return the address of a ButTestSuite.
 * This is the only function that a shared/dynamic-link library (DLL) must
 * export so the test driver can retrieve the test suite.
 * 
 * Yes, you do have to write test cases if you want something more than an
 * empty test suite.
 */
typedef but_test_suite* (but_test_suite_load)(void);

// This is the only function that a Basic Unit Test DLL must export.
#define TEST_SUITE_LOAD_NAME "test_suite_load"
DllExport but_test_suite_load test_suite_load;

END_EXTERN_C

#endif  /* BUT_H_INCLUDED */
