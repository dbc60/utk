/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include "test_but_driver.h"
#include <but_driver.h>
#include <but_version.h>

// For NULL
#include <stddef.h>
// for itoa()
#include <stdlib.h>
// For strcmp(), strncat()
#include <string.h>

#define BTC_NAME_SUCCESS    "Test Success"
#define BTC_NAME_FAIL       "Test Fail"
#define SUITE_NAME          "Driver"

// Names of test cases
#define BTC_NAME_VALIDATE_VERSION   "Validate Version"
#define BTC_NAME_LOAD_DRIVER        "Load Driver"
#define BTC_NAME_NEW_DELETE         "New and Delete Context"
#define BTC_NAME_IS_VALID           "Validate Context"
#define BTC_NAME_NEXT_INDEX_MORE    "Next/Index/More"
#define BTC_NAME_CASE_NAME          "Case Name"
#define BTC_NAME_SUITE_NAME         "Suite Name"
#define BTC_NAME_INDEX              "Index"
#define BTC_NAME_COUNT              "Count"
#define BTC_NAME_RUN                "Run"
#define BTC_NAME_RESULTS            "Results"

TestDriverData testData;

static but_result context_setup(void *data);
static void context_teardown(void *data);

static but_result test_is_valid_version(void *data);
static but_result test_new_delete(void *data);
static but_result test_is_valid_context(void *data);
static but_result test_next_index_more(void *data);
static but_result test_name_case(void *data);
static but_result test_name_suite(void *data);
static but_result test_index(void *data);
static but_result test_count(void *data);
static but_result test_run(void *data);
static but_result test_results(void *data);
static but_test_suite test_suite_success_fail;

but_result
driver_setup(void *data) {
    TestDriverData  *tdd = (TestDriverData*)data;

    tdd->btdd_new = &but_new;
    tdd->btdd_delete = &but_delete;
    tdd->btdd_get_version_str = &but_get_version_str;
    tdd->btdd_get_version_num = &but_get_version_num;
    tdd->btdd_is_valid = &but_is_valid;
    tdd->btdd_next = &but_next;
    tdd->btdd_more = &but_more_test_cases;
    tdd->btdd_get_name_case = &but_get_name_test_case;
    tdd->btdd_get_name_suite = &but_get_name_test_suite;
    tdd->btdd_get_index = &but_get_index;
    tdd->btdd_get_count = &but_get_count_test_cases;
    tdd->btdd_run_current = &but_run;
    tdd->btdd_get_count_passed = &but_get_count_passed;
    tdd->btdd_get_count_failed = &but_get_count_failed;
    tdd->btdd_get_count_failed_setup = &but_get_count_failed_setup;
    tdd->btdd_get_count_results = &but_get_count_results;
    tdd->btdd_get_result = &but_get_result;
    tdd->btdd_ts = &test_suite_success_fail;

    return BUT_SUCCESS;
}


but_test_case test_case_valid_version =
{
    BTC_NAME_VALIDATE_VERSION,
    &driver_setup,
    &test_is_valid_version,
    NULL,
    (void*)&testData
};

but_test_case test_case_new_delete =
{
    BTC_NAME_NEW_DELETE,
    &driver_setup,
    &test_new_delete,
    NULL,
    (void*)&testData
};

but_test_case test_case_valid_context =
{
    BTC_NAME_IS_VALID,
    driver_setup,
    test_is_valid_context,
    NULL,
    (void*)&testData
};

but_test_case test_case_next =
{
    BTC_NAME_NEXT_INDEX_MORE,
    context_setup,
    test_next_index_more,
    context_teardown,
    (void*)&testData
};

but_test_case test_case_name_case =
{
    BTC_NAME_CASE_NAME,
    context_setup,
    test_name_case,
    context_teardown,
    (void*)&testData
};

but_test_case test_case_name_suite =
{
    BTC_NAME_SUITE_NAME,
    context_setup,
    test_name_suite,
    context_teardown,
    (void*)&testData
};

but_test_case test_case_index =
{
    BTC_NAME_INDEX,
    context_setup,
    test_index,
    context_teardown,
    (void*)&testData
};

but_test_case test_case_count =
{
    BTC_NAME_COUNT,
    context_setup,
    test_count,
    context_teardown,
    (void*)&testData
};

but_test_case test_case_run =
{
    BTC_NAME_RUN,
    context_setup,
    test_run,
    context_teardown,
    (void*)&testData
};


but_test_case test_case_results =
{
    BTC_NAME_RESULTS,
    context_setup,
    test_results,
    context_teardown,
    (void*)&testData
};


static but_result test_success(void *data) {
    UNREFERENCED(data);
    return BUT_SUCCESS;
}

static but_result test_fail(void *data) {
    UNREFERENCED(data);
    return BUT_FAIL;
}

static but_test_case test_case_success =
{
    BTC_NAME_SUCCESS,
    NULL,
    test_success,
    NULL,
    NULL
};

static but_test_case test_case_fail =
{
    BTC_NAME_FAIL,
    NULL,
    test_fail,
    NULL,
    NULL
};

static but_test_case *tca[] =
{
    &test_case_success,
    &test_case_fail
};

static but_test_suite test_suite_success_fail =
{
    SUITE_NAME,
    ARRAY_COUNT(tca),
    tca
};


static but_result
context_setup(void *data)
{
    TestDriverData  *tdd = (TestDriverData*)data;
    but_result result = 0;

    result = driver_setup(data);
    if (BUT_SUCCESS == result) {
        tdd->btdd_ctx = tdd->btdd_new(tdd->btdd_ts);
    
        if (!tdd->btdd_is_valid(tdd->btdd_ctx)) {
            result = BUT_FAIL;
        }
    }

    return result;
}


static void
context_teardown(void *data)
{
    TestDriverData *tdd = (TestDriverData*)data;

    tdd->btdd_delete(tdd->btdd_ctx);
}


/**
 * @brief exported unit tests
 */

static but_result
test_is_valid_version(void *data)
{
    but_result result = BUT_FAIL;
    TestDriverData *tdd = (TestDriverData*)data;
    const ch8 *version_str;
    s32 version_num;

    version_str = tdd->btdd_get_version_str();
    version_num = tdd->btdd_get_version_num();
    if (version_str) {
        if (0 == strncmp(version_str,
                         BUT_VERSION_STR,
                         ARRAY_COUNT(BUT_VERSION_STR) - 1)
            && BUT_VERSION_NUM == version_num) {
            result = BUT_SUCCESS;
        }
    }

    return result;
}


static but_result
test_new_delete(void *data)
{
    but_result          result = BUT_FAIL;
    TestDriverData      *tdd = (TestDriverData*)data;

    tdd->btdd_ctx = tdd->btdd_new(tdd->btdd_ts);
    if (tdd->btdd_ctx) {
        tdd->btdd_delete(tdd->btdd_ctx);
        result = BUT_SUCCESS;
    }

    return result;
}


static but_result
test_is_valid_context(void *data)
{
    but_result          result = BUT_FAIL;
    TestDriverData      *tdd = (TestDriverData*)data;

    tdd->btdd_ctx = tdd->btdd_new(tdd->btdd_ts);
    if (tdd->btdd_ctx) {
        if (tdd->btdd_is_valid(tdd->btdd_ctx)) {
            result = BUT_SUCCESS;
        }

        tdd->btdd_delete(tdd->btdd_ctx);
    }

    return result;
}


/**
 * @todo - This is not a good test. It can fail in too many ways. If it doesn't
 * return BUT_SUCCESS, it will take more effort than necessary to figure out
 * why and where it failed.
 */
static but_result
test_next_index_more(void *data)
{
    TestDriverData  *tdd = (TestDriverData*)data;
    but_result      result = BUT_FAIL;

    if (tdd->btdd_get_count(tdd->btdd_ctx) > 0) {
        tdd->btdd_next(tdd->btdd_ctx);
        if (tdd->btdd_get_index(tdd->btdd_ctx) == 1) {
            if (tdd->btdd_more(tdd->btdd_ctx) == 1) {
                tdd->btdd_next(tdd->btdd_ctx);
                if (FALSE == tdd->btdd_more(tdd->btdd_ctx)) {
                    result = BUT_SUCCESS;
                }
            }
        }
    }

    return result;
}


static but_result
test_name_case(void *data)
{
    TestDriverData  *tdd = (TestDriverData*)data;
    but_result      result = BUT_FAIL;
    const ch8      *name;

    name = tdd->btdd_get_name_case(tdd->btdd_ctx);

    if (name && 0 == strcmp(name, BTC_NAME_SUCCESS)) {
        tdd->btdd_next(tdd->btdd_ctx);
        name = tdd->btdd_get_name_case(tdd->btdd_ctx);
        if (name && 0 == strcmp(name, BTC_NAME_FAIL)) {
            result = BUT_SUCCESS;
        }
    }

    return result;
}


static but_result
test_name_suite(void *data)
{
    TestDriverData  *tdd = (TestDriverData*)data;
    but_result      result = BUT_FAIL;
    const ch8      *name;

    name = tdd->btdd_get_name_suite(tdd->btdd_ctx);
    if (name && 0 == strcmp(name, SUITE_NAME)) {
        tdd->btdd_next(tdd->btdd_ctx);
        name = tdd->btdd_get_name_suite(tdd->btdd_ctx);
        if (name && 0 == strcmp(name, SUITE_NAME)) {
            result = BUT_SUCCESS;
        }
    }

    return result;
}


static but_result
test_index(void *data)
{
    TestDriverData  *tdd = (TestDriverData*)data;
    but_result      result = BUT_FAIL;
    size_t          index;

    index = tdd->btdd_get_index(tdd->btdd_ctx);
    if (0 == index) {
        tdd->btdd_next(tdd->btdd_ctx);
        if (tdd->btdd_get_index(tdd->btdd_ctx) == 1) {
            result = BUT_SUCCESS;
        }
    }

    return result;
}


static but_result
test_count(void *data)
{
    TestDriverData  *tdd = (TestDriverData*)data;
    but_result      result = BUT_FAIL;

    if (tdd->btdd_get_count(tdd->btdd_ctx) == 2) {
        result = BUT_SUCCESS;
    }

    return result;
}


static but_result
test_run(void *data)
{
    TestDriverData  *tdd = (TestDriverData*)data;
    but_result      result = BUT_FAIL;

    tdd->btdd_run_current(tdd->btdd_ctx);
    tdd->btdd_next(tdd->btdd_ctx);
    tdd->btdd_run_current(tdd->btdd_ctx);

    if (tdd->btdd_get_count_passed(tdd->btdd_ctx) == 1
        && tdd->btdd_get_count_failed(tdd->btdd_ctx) == 1
        && tdd->btdd_get_count_failed_setup(tdd->btdd_ctx) == 0
        && tdd->btdd_get_count_results(tdd->btdd_ctx) == 1) {
        result = BUT_SUCCESS;
    }

    return result;
}


static but_result
test_results(void *data)
{
    TestDriverData  *tdd = (TestDriverData*)data;
    but_context*    ctx = tdd->btdd_ctx;
    but_result      result = BUT_FAIL;

    tdd->btdd_run_current(ctx);
    tdd->btdd_next(ctx);
    tdd->btdd_run_current(ctx);

    // zero is a successful test, so we expect BUT_PASSED and BUT_FAILED
    if (tdd->btdd_get_result(ctx, 0) == BTR_PASSED
        && tdd->btdd_get_result(ctx, 1) == BTR_FAILED) {
        result = BUT_SUCCESS;
    }

    return result;
}
