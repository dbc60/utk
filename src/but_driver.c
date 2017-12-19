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
    s32                     error_code;    // from the test case
};
typedef struct result_context result_context;


struct test_context
{
    intptr_t        magic;              // indicates a valid context
    but_test_suite *test_suite;         // suite under test
    memory_index    index;              // index of the current test case
    size_t          count_run;          // number of tests run
    size_t          count_passed;       // number of tests that ran and passed
    size_t          count_failed;       // number of tests that ran and failed
    size_t          count_failed_setup; // number of tests that failed setup
    size_t          count_results;      // number of test results
    size_t          capacity;           // number of results that can be stored
    result_context *results;            // array of test results.
};
typedef struct test_context test_context;


/**
 *  Local functions
 */

static void
grow_capacity(test_context *ctx)
{
    result_context *new_results;
    size_t          new_capacity, count;
    size_t const    increment = 10;

    // Calculate a new capacity, but make it no more than
    // the number of test cases in the test suite
    if (ctx->capacity + increment > (size_t)ctx->test_suite->count)
    {
        new_capacity = ctx->test_suite->count;
        count = ctx->test_suite->count - ctx->capacity;
    }
    else
    {
        new_capacity = ctx->capacity + increment;
        count = increment;
    }

    new_results = realloc(ctx->results, new_capacity * sizeof(result_context));
    if (new_results)
    {
        // Initialize new memory block
        memset(&new_results[ctx->capacity],
               0,
               count * sizeof(result_context));
        ctx->capacity = new_capacity;
        ctx->results = new_results;
    }
}   // grow_capacity


static void
insert_result(test_context *ctx, enum but_test_result result, int error_code)
{
    if (ctx->count_results == ctx->capacity)
    {
        grow_capacity(ctx);
    }

    if ((size_t)ctx->count_results < ctx->capacity)
    {
        ctx->results[ctx->count_results].index = ctx->index;
        ctx->results[ctx->count_results].result = result;
        ctx->results[ctx->count_results].error_code = error_code;
        ++ctx->count_results;
    }
}   // insert_result


/**
 * Public functions
 */

/**
 * @brief validate the test context
 * A valid test context is one where the magic number is the address of the
 * context, it has a non-null pointer to a test suite, and its index is no
 * greater than count.
 */
b32
test_context_is_valid(test_context* ctx)
{
    b32 result = FALSE;

    if (ctx) {
        if (ctx->magic == (intptr_t)ctx
            && ctx->test_suite != NULL
            && ctx->index <= ctx->test_suite->count) {
            result = TRUE;
        }
    }

    return result;
}

test_context*
test_context_new(but_test_suite *bts)
{
    test_context *result;

    result = calloc(1, sizeof(test_context));
    if (result != NULL) {
        result->magic = (intptr_t)result;
        result->test_suite = bts;
    }

    return result;
}

void
test_context_delete(test_context *ctx)
{
    if (ctx->results) {
        free(ctx->results);
    }

    free(ctx);
}

void
test_context_next(test_context *ctx)
{
    if (ctx->index < ctx->test_suite->count) {
        ++ctx->index;
    }
}

b32
test_context_more_test_cases(test_context *ctx)
{
    return (ctx->index < ctx->test_suite->count) ? TRUE : FALSE;
}

const ch8 *
test_context_get_name_test_suite(test_context *ctx)
{
    return ctx->test_suite->name;
}

const ch8 *
test_context_get_name_test_case(test_context *ctx)
{
    const ch8* result = NULL;

    if (ctx->index < ctx->test_suite->count) {
        result = ctx->test_suite->test_cases[ctx->index]->name;
    }

    return result;
}

size_t
test_context_get_index(test_context *ctx)
{
    return ctx->index;
}

size_t
test_context_get_count_test_cases(test_context *ctx)
{
    return ctx->test_suite->count;
}

void
test_context_run(test_context *ctx)
{
    if (test_context_more_test_cases(ctx)) {
        but_test_case *tc = ctx->test_suite->test_cases[ctx->index];
        s32 result_setup = 0;
        s32 result_test = 0;

        // a setup routine is optional
        if (tc->setup) {
            // non-zero is an error
            result_setup = tc->setup(tc->test_data);
            if (result_setup) {
                insert_result(ctx, BTR_FAILED_SETUP, result_setup);
                ++ctx->count_failed_setup;
            }
        }

        if (0 == result_setup) {
            result_test = tc->run(tc->test_data);
            if (result_test) {
                insert_result(ctx, BTR_FAILED, result_test);
                ++ctx->count_failed;
            } else {
                ++ctx->count_passed;
            }

            // a teardown routine is optional
            if (tc->teardown) {
                tc->teardown(tc->test_data);
            }
        }

        ++ctx->count_run;
    }
}

size_t
test_context_get_count_run(test_context *ctx)
{
    return ctx->count_run;
}

size_t
test_context_get_count_passed(test_context *ctx)
{
    return ctx->count_passed;
}

size_t
test_context_get_count_failed(test_context *ctx)
{
    return ctx->count_failed;
}

size_t
test_context_get_count_failed_setup(test_context *ctx)
{
    return ctx->count_failed_setup;
}

size_t
test_context_get_count_results(test_context *ctx)
{
    return ctx->count_results;
}

but_test_result
test_context_get_result(test_context *ctx, size_t index)
{
    but_test_result result = BTR_PASSED;
    size_t i = 0;

    if (ctx->count_run <= index) {
        // the test wasn't run
        result = BTR_NOT_RUN;
    } else if (ctx->count_results > 0) {
        // Search the results and see if there's on matching the index
        while (i < ctx->count_results && ctx->results[i].index < index) {
            ++i;
        }

        if (i < ctx->count_results && ctx->results[i].index == index) {
            // We have a match
            result = ctx->results[i].result;
        }
    }

    return result;
}

s32
test_context_get_error_code(test_context *ctx, size_t index)
{
    s32 result = 0;
    size_t i = 0;

    if (index < ctx->count_run && ctx->count_results > 0) {
        // Search the results and see if there's one matching the index
        while (i < ctx->count_results && ctx->results[i].index < index) {
            ++i;
        }

        if (i < ctx->count_results && ctx->results[i].index == index) {
            // We have a match
            result = ctx->results[i].error_code;
        }
    }

    return result;
}
