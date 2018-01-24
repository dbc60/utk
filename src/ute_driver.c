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
#include <ehm.h>


// malloc prototypes
#include <stdlib.h> 

// For ptrdiff_t
#include <stddef.h>

// memset prototype
#include <memory.h>

// Record the invalid address passed to free
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



struct ute_context
{
    intptr_t        magic;              // indicates a valid context
    utk_test_suite *test_suite;         // suite under test
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


/**
 *  Local functions
 */

INTERNAL_FUNCTION void
grow_capacity(ute_context *ctx)
{
    ute_result_context *new_results;
    size_t new_capacity;
    size_t count;
    size_t const increment = 10;

    // Calculate a new capacity, but make it no more than
    // the number of test cases in the test suite
    if (ctx->capacity + increment > (size_t)ctx->test_suite->count) {
        new_capacity = ctx->test_suite->count;
        count = ctx->test_suite->count - ctx->capacity;
    } else {
        new_capacity = ctx->capacity + increment;
        count = increment;
    }

    new_results = realloc(ctx->results,
                          new_capacity * sizeof(ute_result_context));
    if (new_results) {
        // Initialize new memory block
        memset(&new_results[ctx->capacity],
               0,
               count * sizeof(ute_result_context));
        ctx->capacity = new_capacity;
        ctx->results = new_results;
    }
}


INTERNAL_FUNCTION void
insert_result(ute_context *ctx, ute_test_result result, int error_code)
{
    if (ctx->count_results == ctx->capacity) {
        grow_capacity(ctx);
    }

    if ((size_t)ctx->count_results < ctx->capacity) {
        ctx->results[ctx->count_results].index = ctx->index;
        ctx->results[ctx->count_results].result = result;
        ctx->results[ctx->count_results].error_code = error_code;
        ++ctx->count_results;
    }
}


/**
 * Public functions
 */

ute_context*
ute_new(utk_test_suite *ts)
{
    ute_context *result;

    result = calloc(1, sizeof(ute_context));
    if (result != NULL) {
        result->magic = (intptr_t)result;
        result->test_suite = ts;
        ute_counter_init(&result->counter, result);
    }

    return result;
}

void
ute_delete(ute_context *ctx)
{
    if (ctx->results) {
        free(ctx->results);
    }

    free(ctx);
}


/**
 * @brief validate the test context
 * A valid test context is one where the magic number is the address of the
 * context, it has a non-null pointer to a test suite, and its index is no
 * greater than count.
 */
b32
ute_is_valid(ute_context *ctx)
{
    b32 result = FALSE;

    if (ctx) {
        ute_context *counter_ctx = ute_counter_get_context(&ctx->counter);
        if (ctx->magic == (intptr_t)ctx
            && ctx->test_suite != NULL
            && ctx->index <= ctx->test_suite->count
            && ctx == counter_ctx) {
            result = TRUE;
        }
    }

    return result;
}

b32
ute_is_end(ute_context *ctx)
{
    return ctx->index == ctx->test_suite->count;
}

void
ute_next(ute_context *ctx)
{
    if (ctx->index < ctx->test_suite->count) {
        ++ctx->index;
    }
}

const ch8 *
ute_get_name_test_suite(ute_context *ctx)
{
    return ctx->test_suite->name;
}

const ch8 *
ute_get_name_test_case(ute_context *ctx)
{
    const ch8* result = NULL;

    if (ctx->index < ctx->test_suite->count) {
        result = ctx->test_suite->test_cases[ctx->index]->name;
    }

    return result;
}

size_t
ute_get_index(ute_context *ctx)
{
    return ctx->index;
}

size_t
ute_get_count_test_cases(ute_context *ctx)
{
    return ctx->test_suite->count;
}


/**
 * @brief run the current test case for each exception point.
 * Run the setup and teardown methods if they exist. When no test exception is
 * thrown, we have a valid test result to record. For each exception point, we
 * record memory leaks.
 */
void
ute_run(ute_context *ctx)
{
    ute_counter counter;
    utk_test_suite *ts = ctx->test_suite;
    utk_test_case *tc = ts->test_cases[ctx->index];
    u64 count_start;
    utk_result result_setup = UTR_PASSED;
    utk_result result_test = UTR_PASSED;
    b32 thrown = FALSE;
    u64 count_leaks;

    ute_counter_init(&counter, ctx);
    do {
        count_start = ute_get_count_allocation(&counter);
        ute_increment_count_throw(&counter);

        EHM_TRY {
            if (tc->setup) {
                result_setup = tc->setup(tc->test_data);
                if (result_setup != UTR_PASSED) {
                    insert_result(ctx, UTR_SETUP_FAILED, result_setup);
                    ++ctx->count_failed_setup;
                }
            }

            if (UTR_PASSED == result_setup) {
                ++ctx->count_run;
                result_test = tc->run(tc->test_data);
            }
        } EHM_CATCH(exception_ute_test) {
            // Do nothing
        } EHM_ENDTRY;

        thrown = ute_thrown(&counter);
        ute_throw_disable(&counter);

        /**
         * If an exception_ute_test was NOT thrown, then we have a valid result
         * to record.
         */
        if (!thrown) {
            if (result_setup != UTR_PASSED) {
                insert_result(ctx, UTR_SETUP_FAILED, result_setup);
                ++ctx->count_failed_setup;

                // Copy result_setup to result_test to return an error code
                result_test = result_setup;
            }

            if (result_test != UTR_PASSED) {
                insert_result(ctx, UTR_FAILED, result_test);
                ++ctx->count_failed;
            } else {
                ++ctx->count_passed;
            }
        }

        count_leaks = ute_get_count_allocation(&counter) - count_start;
        if (count_leaks > 0) {
            if (!thrown && result_test != UTR_PASSED) {
                /**
                 * If the test result was UTR_PASSED, then change it to
                 * UTR_EXC_FAILED.
                 */
                utk_result result = ute_get_result(ctx, ctx->index);
                if (UTR_PASSED == result) {
                    insert_result(ctx, UTR_EXC_FAILED, result);
                }
            } else {
                insert_result(ctx, UTR_EXC_FAILED, result_test);
            }
        }
    } while (thrown);
}


size_t
ute_get_count_passed(ute_context *ctx)
{
    return ctx->count_passed;
}


/** @brief retrieve the number of tests that passed */
size_t
ute_get_count_failed(ute_context *ctx)
{
    return ctx->count_failed;
}


/** @brief retrieve the number of tests that passed */
size_t
ute_get_count_failed_setup(ute_context *ctx)
{
    return ctx->count_failed_setup;
}


/** @brief retrieve the number of tests that passed */
size_t
ute_get_count_results(ute_context *ctx)
{
    return ctx->count_results;
}

/** @brief retrieve the number of tests that passed */
ute_test_result
ute_get_result(ute_context *ctx, size_t index)
{
    ute_test_result result = UTR_PASSED;
    size_t i = 0;

    if (ctx->count_run <= index) {
        // the test wasn't run
        result = UTR_NOT_RUN;
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
