/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include "test_ute_driver.h"
#include <ute_version.h>
#include <ute_driver.h>
#include <ute_counter.h>

// For NULL
#include <stddef.h>
// For strncmp()
#include <string.h>


// The name of the exported test suite
#define UTE_TS_NAME "UTE"

// The names of our test cases
#define TC_NAME_VALIDATE_VERSION    "UTE Validate Version"
#define TC_NAME_NEW_DELETE          "UTE New and Delete Context"
#define TC_NAME_IS_VALID            "UTE Validate Context"
#define TC_NAME_NEXT_INDEX_END      "UTE Next/Index/End"
#define TC_NAME_CASE_NAME           "UTE Case Name"
#define TC_NAME_SUITE_NAME          "UTE Suite Name"
#define TC_NAME_INDEX               "UTE Index"
#define TC_NAME_COUNT               "UTE Count"
#define TC_NAME_RUN                 "UTE Run"

// The names of internal test cases
#define TC_NAME_SUCCESS "Test Success"
#define TC_NAME_FAIL "Test Fail"
#define TC_NAME_COUNTER_INIT "Test Counter Init"
#define TC_NAME_COUNTER_THROW_1 "Test Counter Throw 1"
#define SUITE_NAME_INTERNAL "Suite Internal"

ute_test_driver_inf test_data;

static utk_result context_setup_success_fail(void *data);
static void context_teardown(void *data);

static utk_result test_is_valid_version(void *data);
static utk_result test_new_delete(void *data);
static utk_result test_is_valid_context(void *data);
static utk_result test_next_index_next_end(void *data);
static utk_result test_name_case(void *data);
static utk_result test_name_suite(void *data);
static utk_result test_index(void *data);
static utk_result test_count(void *data);
static utk_result test_run(void *data);

static utk_test_suite test_suite_internal;

enum test_driver_results {
    TDR_SUCCESS = UTK_SUCCESS,
    TDR_NULL_TEST_SUITE,
    TDR_EXPECTED_FAIL,
    TDR_CONTEXT_INVALID,
    TDR_VERSION_STR_NULL,
    TDR_VERSION_STR_INVALID,
    TDR_VERSION_NUM_INVALID,
    TDR_UNEXPECTED_NULL,
    TDR_UNEXPECTED_COUNT,
    TDR_UNEXPECTED_COUNT_PASSED,
    TDR_UNEXPECTED_COUNT_FAILED,
    TDR_UNEXPECTED_COUNT_FAILED_SETUP,
    TDR_UNEXPECTED_COUNT_RESULTS,
    TDR_UNEXPECTED_INDEX_1,
    TDR_UNEXPECTED_INDEX_2,
    TDR_UNEXPECTED_END,
    TDR_UNEXPECTED_MORE,
    TDR_UNEXPECTED_NAME_1,
    TDR_UNEXPECTED_NAME_2,
    TDR_UNEXPECTED_RESULT_1,
    TDR_UNEXPECTED_RESULT_2,
    TDR_UNEXPECTED_CONTEXT,
    TDR_UNEXPECTED_ENABLED_THROW,
    TDR_UNEXPECTED_DISABLED_THROW,
    TDR_UNEXPECTED_CATCH,
    TDR_UNEXPECTED_THROW,
    TDR_UNEXPECTED_NO_THROW
};


static utk_result
driver_setup(ute_test_driver_inf *tdd, utk_test_suite *ts) {
    utk_result result = UTK_SUCCESS;

    if (ts != NULL) {
        tdd->tdd_get_version_str = &ute_get_version_str;
        tdd->tdd_get_version_num = &ute_get_version_num;
        tdd->tdd_new = &ute_new;
        tdd->tdd_delete = &ute_delete;
        tdd->tdd_is_valid = &ute_is_valid;
        tdd->tdd_next = &ute_next;
        tdd->tdd_is_end = &ute_is_end;
        tdd->tdd_get_name_case = &ute_get_name_test_case;
        tdd->tdd_get_name_suite = &ute_get_name_test_suite;
        tdd->tdd_get_index = &ute_get_index;
        tdd->tdd_get_count = &ute_get_count_test_cases;
        tdd->tdd_run_current = &ute_run;
        tdd->tdd_get_count_passed = &ute_get_count_passed;
        tdd->tdd_get_count_failed = &ute_get_count_failed;
        tdd->tdd_get_count_failed_setup = &ute_get_count_failed_setup;
        tdd->tdd_get_count_results = &ute_get_count_results;
        tdd->tdd_get_result = &ute_get_result;

        tdd->tdd_ts = ts;
    } else {
        result = TDR_NULL_TEST_SUITE;
    }

    return result;
}


static utk_result
driver_setup_success_fail(void *data)
{
    ute_test_driver_inf *tdd = (ute_test_driver_inf*)data;

    return driver_setup(tdd, &test_suite_internal);
}


static utk_result
context_setup(ute_test_driver_inf *tdd)
{
    utk_result result = TDR_SUCCESS;

    tdd->tdd_ctx = tdd->tdd_new(tdd->tdd_ts);

    if (!tdd->tdd_is_valid(tdd->tdd_ctx)) {
        result = TDR_CONTEXT_INVALID;
    }

    return result;
}


static utk_result
context_setup_success_fail(void *data)
{
    ute_test_driver_inf *tdd = (ute_test_driver_inf*)data;
    utk_result result = TDR_SUCCESS;

    result = driver_setup(tdd, &test_suite_internal);
    if (TDR_SUCCESS == result) {
        context_setup(tdd);
    }

    return result;
}


static utk_result
context_setup_counter(void *data)
{
    ute_test_driver_inf *tdd = (ute_test_driver_inf*)data;
    utk_result result = TDR_SUCCESS;

    result = driver_setup(tdd, &test_suite_internal);
    if (TDR_SUCCESS == result) {
        context_setup(tdd);
    }

    return result;
}


static void
context_teardown(void *data)
{
    ute_test_driver_inf *tdd = (ute_test_driver_inf*)data;

    tdd->tdd_delete(tdd->tdd_ctx);
}


utk_test_case test_case_valid_version =
{
    TC_NAME_VALIDATE_VERSION,
    &driver_setup_success_fail,
    &test_is_valid_version,
    NULL,
    (void*)&test_data
};

utk_test_case test_case_new_delete =
{
    TC_NAME_NEW_DELETE,
    &driver_setup_success_fail,
    &test_new_delete,
    NULL,
    (void*)&test_data
};

utk_test_case test_case_valid_context =
{
    TC_NAME_IS_VALID,
    driver_setup_success_fail,
    test_is_valid_context,
    NULL,
    (void*)&test_data
};

utk_test_case test_case_index_next_end =
{
    TC_NAME_NEXT_INDEX_END,
    driver_setup_success_fail,
    test_next_index_next_end,
    NULL,
    (void*)&test_data
};

utk_test_case test_case_name_case =
{
    TC_NAME_CASE_NAME,
    context_setup_success_fail,
    test_name_case,
    context_teardown,
    (void*)&test_data
};

utk_test_case test_case_name_suite =
{
    TC_NAME_SUITE_NAME,
    context_setup_success_fail,
    test_name_suite,
    context_teardown,
    (void*)&test_data
};

utk_test_case test_case_index =
{
    TC_NAME_INDEX,
    context_setup_success_fail,
    test_index,
    context_teardown,
    (void*)&test_data
};

utk_test_case test_case_count =
{
    TC_NAME_COUNT,
    context_setup_success_fail,
    test_count,
    context_teardown,
    (void*)&test_data
};

utk_test_case test_case_run = 
{
    TC_NAME_RUN,
    context_setup_counter,
    test_run,
    context_teardown,
    (void*)&test_data
};

/**
 * @brief unit tests
 */

static utk_result
test_is_valid_version(void *data)
{
    utk_result result = TDR_SUCCESS;
    ute_test_driver_inf *tdd = (ute_test_driver_inf*)data;
    const ch8 *version_str;
    s32 version_num;

    version_str = tdd->tdd_get_version_str();
    version_num = tdd->tdd_get_version_num();

    if (NULL == version_str) {
        result = TDR_VERSION_STR_NULL;
    } else if (0 != strncmp(version_str,
                            UTE_VERSION_STR,
                            ARRAY_COUNT(UTE_VERSION_STR) - 1)) {
        result = TDR_VERSION_STR_INVALID;
    } else if (version_num != UTE_VERSION_NUM) {
        result = TDR_VERSION_NUM_INVALID;
    }

    return result;
}


static utk_result
test_new_delete(void *data)
{
    utk_result result = TDR_SUCCESS;
    ute_test_driver_inf * tdd = (ute_test_driver_inf*)data;

    tdd->tdd_ctx = tdd->tdd_new(tdd->tdd_ts);
    if (NULL == tdd->tdd_ctx) {
        result = TDR_UNEXPECTED_NULL;
    } else {
        tdd->tdd_delete(tdd->tdd_ctx);
    }

    return result;
}


static utk_result
test_is_valid_context(void *data)
{
    utk_result result = TDR_SUCCESS;
    ute_test_driver_inf * tdd = (ute_test_driver_inf*)data;

    tdd->tdd_ctx = tdd->tdd_new(tdd->tdd_ts);
    if (NULL == tdd->tdd_ctx) {
        result = TDR_UNEXPECTED_NULL;
    } else if (!tdd->tdd_is_valid(tdd->tdd_ctx)) {
        result = TDR_CONTEXT_INVALID;
        tdd->tdd_delete(tdd->tdd_ctx);
    }

    return result;
}

static utk_result
test_next_index_next_end(void *data)
{
    utk_result result = TDR_SUCCESS;
    ute_test_driver_inf * tdd = (ute_test_driver_inf*)data;

    if (tdd->tdd_get_count(tdd->tdd_ctx) == 0) {
        result = TDR_UNEXPECTED_COUNT;
    } else {
        tdd->tdd_next(tdd->tdd_ctx);
        if (tdd->tdd_get_index(tdd->tdd_ctx) != 1) {
            result = TDR_UNEXPECTED_INDEX_1;
        } else if (tdd->tdd_is_end(tdd->tdd_ctx)) {
            result = TDR_UNEXPECTED_END;
        }
    }

    return result;
}


static utk_result
test_name_case(void *data)
{
    ute_test_driver_inf * tdd = (ute_test_driver_inf*)data;
    utk_result result = TDR_SUCCESS;
    const ch8  * name;

    name = tdd->tdd_get_name_case(tdd->tdd_ctx);

    if (NULL == name) {
        result = TDR_UNEXPECTED_NULL;
    } else if (strcmp(name, TC_NAME_SUCCESS)) {
        result = TDR_UNEXPECTED_NAME_1;
    } else {
        tdd->tdd_next(tdd->tdd_ctx);
        name = tdd->tdd_get_name_case(tdd->tdd_ctx);
        if (NULL == name) {
            result = TDR_UNEXPECTED_NULL;
        } else if (strcmp(name, TC_NAME_FAIL)) {
            result = TDR_UNEXPECTED_NAME_2;
        }
    }

    return result;
}


static utk_result
test_name_suite(void *data)
{
    ute_test_driver_inf * tdd = (ute_test_driver_inf*)data;
    utk_result result = TDR_SUCCESS;
    const ch8 *name;

    name = tdd->tdd_get_name_suite(tdd->tdd_ctx);
    if (NULL == name) {
        result = TDR_UNEXPECTED_NULL;
    } else if (strcmp(name, SUITE_NAME_INTERNAL)) {
        result = TDR_UNEXPECTED_NAME_1;
    } else {
        tdd->tdd_next(tdd->tdd_ctx);
        name = tdd->tdd_get_name_suite(tdd->tdd_ctx);
        if (NULL == name) {
            result = TDR_UNEXPECTED_NULL;
        } else if (strcmp(name, SUITE_NAME_INTERNAL)) {
            result = TDR_UNEXPECTED_NAME_2;
        }
    }

    return result;
}


static utk_result
test_index(void *data)
{
    ute_test_driver_inf * tdd = (ute_test_driver_inf*)data;
    utk_result result = TDR_SUCCESS;
    size_t index;

    index = tdd->tdd_get_index(tdd->tdd_ctx);
    if (index != 0) {
        result = TDR_UNEXPECTED_INDEX_1;
    } else {
        tdd->tdd_next(tdd->tdd_ctx);
        index = tdd->tdd_get_index(tdd->tdd_ctx);
        if (index != 1) {
            result = TDR_UNEXPECTED_INDEX_2;
        }
    }

    return result;
}

static utk_result test_success(void *data) {
    UNREFERENCED(data);
    return TDR_SUCCESS;
}

static utk_result test_fail(void *data) {
    UNREFERENCED(data);
    return TDR_EXPECTED_FAIL;
}

static utk_test_case test_case_success =
{
    TC_NAME_SUCCESS,
    NULL,
    test_success,
    NULL,
    NULL
};

static utk_test_case test_case_fail =
{
    TC_NAME_FAIL,
    NULL,
    test_fail,
    NULL,
    NULL
};


static ute_counter test_data_ute_counter;

static utk_result
context_setup_counter_init(void *data)
{
    ute_counter *ctr = (ute_counter*)data;
    ute_counter_init(ctr, NULL);

    return TDR_SUCCESS;
}

/**
* @brief test that UTE counters are initialized properly
*/
static utk_result test_counter_init(void *data)
{
    ute_counter *ctr = (ute_counter*)data;
    utk_result result = UTK_SUCCESS;

    if (ctr->context != NULL) {
        result = TDR_UNEXPECTED_CONTEXT;
    } else if (ctr->count_exception_point != 0
               || ctr->count_throw != 0
               || ctr->count_allocations != 0
               || ctr->count_invalid_free != 0) {
        result = TDR_UNEXPECTED_COUNT;
    } else if (ctr->throw_test_exception) {
        result = TDR_UNEXPECTED_ENABLED_THROW;
    }

    return result;
}


static utk_test_case test_case_counter_init =
{
    TC_NAME_COUNTER_INIT,
    context_setup_counter_init,
    test_counter_init,
    NULL,   // no teardown required
    &test_data_ute_counter
};


static utk_result
context_setup_counter_throw_1(void *data)
{
    ute_counter *ctr = (ute_counter*)data;
    ute_counter_init(ctr, NULL);

    return TDR_SUCCESS;
}

/**
* @brief test that UTE counters are initialized properly
*/
static utk_result
test_counter_throw_1(void *data)
{
    ute_counter *ctr = (ute_counter*)data;
    utk_result result = UTK_SUCCESS;
    const counter32 count_throw = 3;
    counter32 count_throw_attempts = count_throw;

    EHM_TRY {
        EHM_TRY {
            // bump the exception point to count_throw
            for (count_throw_attempts = 0; count_throw_attempts < count_throw; ++count_throw_attempts) {
                ute_increment_count_throw(ctr);
            }

            count_throw_attempts = 0;
            while (count_throw_attempts < count_throw) {
                ++count_throw_attempts;
                ute_throw_try(ctr);
            }

            // We should NOT get here
            result = TDR_UNEXPECTED_NO_THROW;
        } EHM_CATCH(exception_ute_test) {
            /**
             * Linux (gcc 5.4.0 20160609) restores count_throw_attempts to its
             * initialized value. Windows (VS2017) does not. The only reason
             * this test works on Windows is ute_throw_try(ctr) throws the test
             * exception when count_throw_attempts == 1.
             */
            if (count_throw_attempts != count_throw || !ute_thrown(ctr)) {
                result = TDR_UNEXPECTED_CATCH;
            }
        } EHM_ENDTRY;
    } EHM_CATCH_ALL {
        result = TDR_UNEXPECTED_CATCH;
    } EHM_ENDTRY;

    if (!ute_thrown(ctr)) {
        result = TDR_UNEXPECTED_NO_THROW;
    }

    return result;
}

static utk_test_case test_case_counter_throw_1 =
{
    TC_NAME_COUNTER_THROW_1,
    context_setup_counter_throw_1,
    test_counter_throw_1,
    NULL,
    &test_data_ute_counter
};

static utk_test_case *test_case_internal_array[] =
{
    &test_case_success,
    &test_case_fail,
    &test_case_counter_init,
    &test_case_counter_throw_1
};

static utk_result
test_count(void *data)
{
    ute_test_driver_inf  *tdd = (ute_test_driver_inf*)data;
    utk_result result = TDR_SUCCESS;

    if (tdd->tdd_get_count(tdd->tdd_ctx)
        != ARRAY_COUNT(test_case_internal_array)) {
        result = TDR_UNEXPECTED_COUNT;
    }

    return result;
}


static utk_result
test_run(void *data)
{
    ute_test_driver_inf *tdd = (ute_test_driver_inf*)data;
    ute_context *ctx = tdd->tdd_ctx;
    utk_result  result = TDR_SUCCESS;
    u64 count_passed;
    u64 count_failed;
    u64 count_failed_setup;
    u64 count_results;

    while (!tdd->tdd_is_end(ctx)) {
        tdd->tdd_run_current(ctx);
        tdd->tdd_next(ctx);
    }

    count_passed = tdd->tdd_get_count_passed(ctx);
    count_failed = tdd->tdd_get_count_failed(ctx);
    count_failed_setup = tdd->tdd_get_count_failed_setup(ctx);
    count_results = tdd->tdd_get_count_results(ctx);

    if (tdd->tdd_is_end(ctx)) {
        if (count_passed
            != ARRAY_COUNT(test_case_internal_array) - 1) {
            // Correct as long as this is the last test case
            result = TDR_UNEXPECTED_COUNT_PASSED;
        } else if (count_failed != 1) {
            // There is one failing test, so we shouldn't be here
            result = TDR_UNEXPECTED_COUNT_FAILED;
        } else if (count_failed_setup != 0) {
            // No setup methods should fail
            result = TDR_UNEXPECTED_COUNT_FAILED_SETUP;
        } else if (count_results != 1) {
            // There is one failing test, so we shouldn't be here
            result = TDR_UNEXPECTED_COUNT_RESULTS;
        }
    } else {
        result = TDR_UNEXPECTED_MORE;
    }

    return result;
}


static utk_test_suite test_suite_internal =
{
    SUITE_NAME_INTERNAL,
    ARRAY_COUNT(test_case_internal_array),
    test_case_internal_array
};


LOCAL_VARIABLE utk_test_case *tca[] = 
{
    // UTE driver tests
    &test_case_valid_version,
    &test_case_new_delete,
    &test_case_valid_context,
    &test_case_index_next_end,
    &test_case_name_case,
    &test_case_name_suite,
    &test_case_index,
    &test_case_count,
    &test_case_run
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
