/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include "platform.h"
#include "but_internal.h"
#include "but_driver.h"
// malloc prototypes
#include <stdlib.h> 

// For ptrdiff_t
#include <stddef.h>

// memset prototype
#include <memory.h>


/*
 * Point to one test case and the current result from running it.
 * The struct result_context only exists for tests that didn't run or ran, but didn't
 * pass. All tests that run and pass don't generate one of these.
 */
struct result_context
{
    memory_index            index;          // the index of the test case
    enum but_test_result    result;         // the result code
    s32                     result_code;    // from the test case
};


struct test_context
{
    intptr_t                magic;              // indicates a valid context
    struct but_test_suite  *test_suite;          // suite under test
    memory_index            index;              // index of the current test case
    size_t                  runCount;           // number of tests run
    size_t                  passCount;          // number of tests that ran and passed
    size_t                  failCount;          // number of tests that ran and failed
    size_t                  setupFailCount;     // number of tests that failed setup
    size_t                  resultsCount;       // number of test results
    size_t                  resultsCapacity;    // number of results that can be stored
    struct result_context* results;             // array of test results.
};


/**
 *  Local functions
 */

static void
growCapacity(struct test_context *ctx)
{
    struct result_context  *new_results;
    size_t                  new_capacity, count;
    size_t const            increment = 10;

    // Calculate a new capacity, but make it no more than
    // the number of test cases in the test suite
    if (ctx->resultsCapacity + increment > (size_t)ctx->test_suite->count)
    {
        new_capacity = ctx->test_suite->count;
        count = ctx->test_suite->count - ctx->resultsCapacity;
    }
    else
    {
        new_capacity = ctx->resultsCapacity + increment;
        count = increment;
    }

    new_results = realloc(ctx->results, new_capacity * sizeof(struct result_context));
    if (new_results)
    {
        // Initialize new memory block
        memset(&new_results[ctx->resultsCapacity],
               0,
               count * sizeof(struct result_context));
        ctx->resultsCapacity = new_capacity;
        ctx->results = new_results;
    }
}   // growCapacity


static void
addResult(struct test_context *ctx, enum but_test_result result, int result_code)
{
    if (ctx->resultsCount == ctx->resultsCapacity)
    {
        growCapacity(ctx);
    }

    if ((size_t)ctx->resultsCount < ctx->resultsCapacity)
    {
        ctx->results[ctx->resultsCount].index = ctx->index;
        ctx->results[ctx->resultsCount].result = result;
        ctx->results[ctx->resultsCount].result_code = result_code;
        ++ctx->resultsCount;
    }
}   // addResult


/**
 * Public functions
 */

