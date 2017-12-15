/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include "platform.h"
#include "but.h"
#include "but_driver.h"

/*
 * Point to one test case and the current result from running it.
 * The ResultContext only exists for tests that didn't run or ran, but didn't
 * pass. All tests that run and pass don't generate one of these.
 */
struct _result_context
{
    memory_index            index;      // the index of the test case
    enum but_test_result    result;     // the result code
    s32                     errorCode;  // the error code from the test case
};


struct test_context
{
    intptr_t                magic;              // indicates a valid context
    struct _but_test_suite* testSuite;          // suite under test
    memory_index            index;              // index of the current test case
    size_t                  runCount;           // number of tests run
    size_t                  passCount;          // number of tests that ran and passed
    size_t                  failCount;          // number of tests that ran and failed
    size_t                  setupFailCount;     // number of tests that failed setup
    size_t                  resultsCount;       // number of test results
    size_t                  resultsCapacity;    // number of results that can be stored
    struct _result_context* results;            // array of test results.
};
