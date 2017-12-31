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

#ifndef UTE_H_INCLUDED
#define UTE_H_INCLUDED

#include "platform.h"

BEGIN_EXTERN_C
// Unit Test Extended (UTE)

// Forward declarations of opaque types
typedef void* ute_test_data;
typedef struct ute_context ute_context;
typedef struct ute_test_suite ute_test_suite;
typedef struct ute_test_case ute_test_case;

/*
 * ute_test_result is ordered from best result (passed) to successively worse
 * results, ending in not run. Subjectively, memory leaks along exception paths
 * are worse than passing along all paths. Memory leaks along the main path are
 * worse than along the exception path. A test failure along an exception path
 * is worse than any memory leaks. A test failure along the main path is worse
 * than along an exception path. If we didn't get to run the test because the
 * setup code failed, then that's even worse. If the test context has an error,
 * then something is wrong with the test driver. The reason the "not run"
 * condition is at the end is arbitrary.
 */
enum ute_test_results
{
    // The test case was run and it returned successfully along all paths
    UTR_PASSED,

    // The test case passed, but leaked memory on an exception path
    UTR_EXC_WEAK_PASS,

    // The test case passed, but leaked memory on the main path
    UTR_WEAK_PASS,

    // The main test passed, but an exception path failed
    UTR_EXC_FAILED,

    // The test case was run and it returned a failure on the main path
    UTR_FAILED,

    // The setup function reported a failure
    UTR_SETUP_FAILED,

    // This is an internal error. Something is wrong with the test context
    UTR_CONTEXT_INVALID,

    // The test case has not run
    UTR_NOT_RUN
};
typedef enum ute_test_results ute_test_result;

enum ute_results {
    UTe_FAIL,
    UTe_SUCCESS
};
typedef enum ute_results ute_result;

typedef ute_result  (ute_method_setup)(void *data);
typedef ute_result  (ute_method_run)(void *data);
typedef void        (ute_method_teardown)(void *data);

#endif  // UTE_H_INCLUDED
