/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include <but.h>
#include "test_ute_counter.h"
#include <ute_driver.h>

#include <string.h>


// The names of our test cases
#define TEST_NAME_INIT  "Initialize Counter"

// The name of the test suite and a forward reference to it.
#define UTE_TS_NAME "UTE Counter"
LOCAL_VARIABLE utk_test_suite ute_ts;

LOCAL_VARIABLE ute_counter_data test_data;

/** @brief test cases for the UTE Counter implementation */

utk_test_case test_case_counter_init =
{
    TEST_NAME_INIT,
    setup_counter,
    test_initialization,
    teardown_counter,
    &test_data
};

utk_test_case test_case_counter_get_context =
{
    TEST_NAME_INIT,
    setup_counter,
    test_initialization,
    teardown_counter,
    &test_data
};

enum test_results {
    CTR_SUCCESS = UTK_SUCCESS,
    CTR_INVALID_EXCEPTION_POINT_INITIAL,
    CTR_INVALID_COUNT_THROW_INITIAL,
    CTR_INVALID_THROW_TEST_EXCEPTION_INITIAL,
    CTR_INVALID_COUNT_ALLOCATIONS_INITIAL,
    CTR_INVALID_COUNT_INVALID_FREE_INITIAL,
    CTR_INVALID_CONTEXT
};


utk_result
setup_counter(void *data)
{
    utk_result result = CTR_SUCCESS;
    ute_counter_data *ctr_data = (ute_counter_data*)data;
    ctr_data->ctx = ute_new(&ute_ts);

    if (NULL == ctr_data->ctx) {
        result = UTK_FAIL_SETUP;
    }

    ute_counter_init(&ctr_data->ctr, ctr_data->ctx);

    return result;
}

void
teardown_counter(void *data)
{
    ute_counter_data *ctr_data = (ute_counter_data*)data;
    ute_context *ctx = ute_counter_get_context(&ctr_data->ctr);
    ute_delete(ctx);
}


/**
 * @brief Initialize a counter and verify the field values are correct.
 */
utk_result
test_initialization(void *data)
{
    utk_result result = CTR_SUCCESS;

    ute_counter_data *ctr_data = (ute_counter_data*)data;

    if (ctr_data->ctr.count_exception_point != 0) {
        result = CTR_INVALID_EXCEPTION_POINT_INITIAL;
    } else if (ctr_data->ctr.count_throw != 0) {
        result = CTR_INVALID_COUNT_THROW_INITIAL;
    } else if (ctr_data->ctr.throw_test_exception) {
        result = CTR_INVALID_THROW_TEST_EXCEPTION_INITIAL;
    } else if (ctr_data->ctr.count_allocations != 0) {
        result = CTR_INVALID_COUNT_ALLOCATIONS_INITIAL;
    } else if (ctr_data->ctr.count_invalid_free != 0) {
        result = CTR_INVALID_COUNT_INVALID_FREE_INITIAL;
    } else if (ctr_data->ctr.context != ctr_data->ctx) {
        result = CTR_INVALID_CONTEXT;
    }

    return result;
}


utk_result
test_get_context(void *data)
{
    ute_counter_data *ctr_data = (ute_counter_data*)data;
    utk_result  result = CTR_SUCCESS;
    ute_context *ctx = ute_counter_get_context(&ctr_data->ctr);

    if (ctr_data->ctx != ctx) {
        result = CTR_INVALID_CONTEXT;
    }

    return result;
}


// The array of test cases for the test suite
LOCAL_VARIABLE
utk_test_case *tca[] = 
{
    // UTE driver tests
    &test_case_counter_init,
    &test_case_counter_get_context
};

LOCAL_VARIABLE
utk_test_suite ute_ts = {UTE_TS_NAME, 
                         ARRAY_COUNT(tca),
                         tca};

DLL_EXPORT utk_test_suite *
test_suite_load(void)
{
    return &ute_ts;
}
