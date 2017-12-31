/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include <ute.h>
#include <ute_driver.h>
#include "ute_counter.h"

// malloc prototypes
#include <stdlib.h> 

// For ptrdiff_t
#include <stddef.h>

// memset prototype
#include <memory.h>


// Record the invalid address passed to free
typedef
struct invalid_free
{
    void   *block;
};
typedef struct invalid_free invalid_free;


/*
** Point to one test case and the current result from running it.
** The ResultContext only exists for tests that didn't run or ran, but didn't
** pass. All tests that run and pass don't generate one of these.
*/
struct ute_result_context
{
    memory_index    index;              // the index of the test case
    ute_test_result result;             // the result code
    int             error_code;         // the error code from the test case
    int             count_fail_free;    // The number of times free failed
    int             capacity;           // The number of invalid addresses that
                                        // can be stored in invalid_addresses
    invalid_free   *invalid_addresses;  // array of invalid addresses
};
typedef struct ute_result_context ute_result_context;



struct ute_test_context
{
    intptr_t        magic;              // indicates a valid context
    ute_test_suite *testSuite;          // suite under test
    memory_index    index;              // index of the current test case
    size_t          count_run;          // number of tests run
    size_t          count_passed;       // number of tests that ran and passed
    size_t          count_failed;       // number of tests that ran and failed
    size_t          count_failed_setup; // number of tests that failed setup
    size_t          count_results;      // number of test results
    size_t          capacity;           // number of results that can be stored
    ute_result_context *results;        // array of test results.
    ute_counter     counter;
};
typedef struct ute_test_context ute_test_context;
