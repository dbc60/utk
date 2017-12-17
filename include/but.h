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

// A Handle for test data
DEFINE_HANDLE(test_data);
DEFINE_HANDLE(test_context);
DEFINE_HANDLE(but_test_case);
DEFINE_HANDLE(but_test_suite);


enum but_test_result
{
    BTR_PASSED,         // The test case was run and it returned successfully
    BTR_FAILED,         // The test case was run and it returned a failure
    BTR_EXC_FAILED,     // The main test passed, but an exception path failed
    BTR_SETUP_FAILED,   // The setup function reported a failure
    BTR_NOTRUN          // The test case has not run
};

typedef wch*    (*test_case_name_get)(void);
typedef s32     (*test_case_setup)(test_data*);
typedef s32     (*test_case_run)(test_data*);
typedef void    (*test_case_teardown)(test_data*);

// Test case interface
struct but_test_case_inf
{
    test_case_name_get  get;
    test_case_setup     setup;
    test_case_run       run;
    test_case_teardown  teardown;
};

typedef const wch* (*test_suite_name_get)(void);
typedef size_t (*test_case_count)(void);
typedef struct but_test_case_inf* (*test_case_get)(size_t index);
typedef struct but_test_case_inf* (*test_case_first)(void);
typedef struct but_test_case_inf* (*test_case_next)(but_test_case_inf);
typedef struct but_test_case_inf* (*test_case_end)(void);

struct but_test_suite_inf
{
    test_suite_name_get name;
    test_case_count     count;
    test_case_get       get;
    test_case_first     first;
    test_case_next      next;
    test_case_end       end;
};

/**
 * A pointer to a function that will return the address of a ButTestSuite.
 * This is the only function that a shared/dynamic-link library (DLL) must
 * export so the test driver can retrieve the test suite.
 */
typedef but_test_suite* (*but_test_suite_get)(void);

// getButTestSuite is the only function that a Basic Unit Test DLL must export.
but_test_suite_get test_suite_get;

END_EXTERN_C

#endif  /* BUT_H_INCLUDED */
