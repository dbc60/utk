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

#ifndef UTK_H_INCLUDED
#define UTK_H_INCLUDED

#include "platform.h"

BEGIN_EXTERN_C

enum utk_results {
    UTK_FAIL,
    UTK_SUCCESS
};
typedef enum utk_results utk_result;

typedef utk_result  (utk_method_setup)(void *data);
typedef utk_result  (utk_method_run)(void *data);
typedef void        (utk_method_teardown)(void *data);

// Test case interface
struct utk_test_case
{
    const ch8           *name;
    utk_method_setup    *setup;
    utk_method_run      *run;
    utk_method_teardown *teardown;
    void                *test_data;
};
typedef struct utk_test_case utk_test_case;

struct utk_test_suite
{
    const ch8          *name;
    size_t              count;
    utk_test_case     **test_cases;
};
typedef struct utk_test_suite utk_test_suite;


/**
 * A pointer to a function that will return the address of a ButTestSuite.
 * This is the only function that a shared/dynamic-link library (DLL) must
 * export so the test driver can retrieve the test suite.
 * 
 * Yes, you do have to write test cases if you want something more than an
 * empty test suite.
 */
typedef utk_test_suite* (utk_test_suite_load)(void);

// This is the only function that a Basic Unit Test DLL must export.
#define TEST_SUITE_LOAD_NAME "test_suite_load"
DllExport utk_test_suite_load test_suite_load;


END_EXTERN_C

#endif  // UTK_H_INCLUDED
