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
#include "utk.h"


BEGIN_EXTERN_C
// Unit Test Extended (UTE)

/** @brief the test context */
typedef struct ute_context ute_context;

/*
 * ute_test_results are ordered from best result (passed) to successively worse
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
    UTR_PASSED_WEAK_EXCEPTION_PATH,

    // The test case passed, but leaked memory on the main path
    UTR_PASSED_WEAK_MAIN,

    // The main test passed, but an exception path failed
    UTR_FAILED_EXCEPTION_PATH,

    // The test case was run and it returned a failure on the main path
    UTR_FAILED_MAIN_PATH,

    // The setup function reported a failure
    UTR_FAILED_SETUP,

    // This is an internal error. Something is wrong with the test context
    UTR_INVALID_CONTEXT,

    // The test case has not run
    UTR_NOT_RUN
};
typedef enum ute_test_results ute_test_result;


END_EXTERN_C

#endif  // UTE_H_INCLUDED
