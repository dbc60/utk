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
typedef struct ute_context ute_context;

/**
 *  Local functions
 */

//INTERNAL_FUNCTION void
//grow_capacity(ute_context *ctx)
//{
//    ute_result_context *new_results;
//    size_t new_capacity;
//    size_t count;
//    size_t const increment = 10;
//
//    // Calculate a new capacity, but make it no more than
//    // the number of test cases in the test suite
//    if (ctx->capacity + increment > (size_t)ctx->test_suite->count) {
//        new_capacity = ctx->test_suite->count;
//        count = ctx->test_suite->count - ctx->capacity;
//    } else {
//        new_capacity = ctx->capacity + increment;
//        count = increment;
//    }
//
//    new_results = realloc(ctx->results,
//                          new_capacity * sizeof(ute_result_context));
//    if (new_results) {
//        // Initialize new memory block
//        memset(&new_results[ctx->capacity],
//               0,
//               count * sizeof(ute_result_context));
//        ctx->capacity = new_capacity;
//        ctx->results = new_results;
//    }
//}


//INTERNAL_FUNCTION void
//insert_result(ute_context *ctx, ute_test_result result, int error_code)
//{
//    if (ctx->count_results == ctx->capacity) {
//        grow_capacity(ctx);
//    }
//
//    if ((size_t)ctx->count_results < ctx->capacity) {
//        ctx->results[ctx->count_results].index = ctx->index;
//        ctx->results[ctx->count_results].result = result;
//        ctx->results[ctx->count_results].error_code = error_code;
//        ++ctx->count_results;
//    }
//}


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
ute_is_valid(ute_context* ctx)
{
    b32 result = FALSE;

    if (ctx) {
        ute_context * counter_ctx = ute_counter_get_context(&ctx->counter);
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


/** @brief run the current test case, including the setup and teardown methods
 * if they exist
 */
void
ute_run(ute_context *ctx)
{
    ute_counter counter;
    //u64 count_start;
    b32 thrown = FALSE;

    ute_counter_init(&counter, ctx);
    do {
        //count_start = ute_get_count_allocation(&counter);
        ute_increment_count_throw(&counter);
    } while (thrown);
}
