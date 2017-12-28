/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include "test_driver.h"
#include <but_driver.h>
#include <but_version.h>

// For NULL
#include <stddef.h>
// For strcmp()
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

TestDriveData testData;

static but_result test_success(void *data);
static but_result test_case_2(void *data);

static but_result driver_setup(void *data);

static but_result context_setup(void *data);
static void context_teardown(void *data);

static but_result test_version_valid(void *data);
static but_result test_new_delete(void *data);
static but_result test_is_valid(void *data);
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
    TestDriveData  *tdd = (TestDriveData*)data;

    tdd->tdd_new = &but_new;
    tdd->tdd_delete = &but_delete;
    tdd->tdd_get_version_str = &but_version;
    tdd->tdd_get_version_num = &but_version_num;
    tdd->tdd_is_valid = &but_is_valid;
    tdd->tdd_next = &but_next;
    tdd->tdd_more = &but_more_test_cases;
    tdd->tdd_get_name_case = &but_get_name_test_case;
    tdd->tdd_get_name_suite = &but_get_name_test_suite;
    tdd->tdd_get_index = &but_get_index;
    tdd->tdd_get_count = &but_get_count_test_cases;
    tdd->tdd_run_current = &but_run;
    tdd->tdd_get_count_passed = &but_get_count_passed;
    tdd->tdd_get_count_failed = &but_get_count_failed;
    tdd->tdd_get_count_failed_setup = &but_get_count_failed_setup;
    tdd->tdd_get_count_results = &but_get_count_results;
    tdd->tdd_get_result = &but_get_result;
    tdd->tdd_ts = &test_suite_success_fail;

    return BUT_SUCCESS;
}


but_test_case test_case_valid_version =
{
    BTC_NAME_VALIDATE_VERSION,
    &driver_setup,
    &test_version_valid,
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
    test_is_valid,
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
    TestDriveData  *tdd = (TestDriveData*)data;
    but_result result = 0;

    result = driver_setup(data);
    if (BUT_SUCCESS == result) {
        tdd->tdd_ctx = tdd->tdd_new(tdd->tdd_ts);
    
        if (!tdd->tdd_is_valid(tdd->tdd_ctx)) {
            result = BUT_FAIL;
        }
    }

    return result;
}


static void
context_teardown(void *data)
{
    TestDriveData *tdd = (TestDriveData*)data;

    tdd->tdd_delete(tdd->tdd_ctx);
}


/**
 * @brief exported unit tests
 */

static but_result
test_version_valid(void *data)
{
    but_result result = BUT_FAIL;
    TestDriveData *tdd = (TestDriveData*)data;
    const ch8 *version_str;
    s32 version_num;

    version_str = tdd->tdd_get_version_str();
    version_num = tdd->tdd_get_version_num();
    if (version_str) {
        if (0 == strncmp(version_str,
                         BUT_VERSION,
                         ARRAY_COUNT(BUT_VERSION) - 1)
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
    TestDriveData      *tdd = (TestDriveData*)data;

    tdd->tdd_ctx = tdd->tdd_new(tdd->tdd_ts);
    if (tdd->tdd_ctx) {
        tdd->tdd_delete(tdd->tdd_ctx);
        result = BUT_SUCCESS;
    }

    return result;
}


static but_result
test_is_valid(void *data)
{
    but_result          result = BUT_FAIL;
    TestDriveData      *tdd = (TestDriveData*)data;

    tdd->tdd_ctx = tdd->tdd_new(tdd->tdd_ts);
    if (tdd->tdd_ctx) {
        if (tdd->tdd_is_valid(tdd->tdd_ctx)) {
            result = BUT_SUCCESS;
        }

        tdd->tdd_delete(tdd->tdd_ctx);
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
    TestDriveData  *tdd = (TestDriveData*)data;
    but_result      result = BUT_FAIL;

    if (tdd->tdd_get_count(tdd->tdd_ctx) > 0) {
        tdd->tdd_next(tdd->tdd_ctx);
        if (tdd->tdd_get_index(tdd->tdd_ctx) == 1) {
            if (tdd->tdd_more(tdd->tdd_ctx) == 1) {
                tdd->tdd_next(tdd->tdd_ctx);
                if (FALSE == tdd->tdd_more(tdd->tdd_ctx)) {
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
    TestDriveData  *tdd = (TestDriveData*)data;
    but_result      result = BUT_FAIL;
    const ch8      *name;

    name = tdd->tdd_get_name_case(tdd->tdd_ctx);

    if (name && 0 == strcmp(name, BTC_NAME_SUCCESS)) {
        tdd->tdd_next(tdd->tdd_ctx);
        name = tdd->tdd_get_name_case(tdd->tdd_ctx);
        if (name && 0 == strcmp(name, BTC_NAME_FAIL)) {
            result = BUT_SUCCESS;
        }
    }

    return result;
}


static but_result
test_name_suite(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    but_result      result = BUT_FAIL;
    const ch8      *name;

    name = tdd->tdd_get_name_suite(tdd->tdd_ctx);
    if (name && 0 == strcmp(name, SUITE_NAME)) {
        tdd->tdd_next(tdd->tdd_ctx);
        name = tdd->tdd_get_name_suite(tdd->tdd_ctx);
        if (name && 0 == strcmp(name, SUITE_NAME)) {
            result = BUT_SUCCESS;
        }
    }

    return result;
}


static but_result
test_index(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    but_result      result = BUT_FAIL;
    size_t          index;

    index = tdd->tdd_get_index(tdd->tdd_ctx);
    if (0 == index) {
        tdd->tdd_next(tdd->tdd_ctx);
        if (tdd->tdd_get_index(tdd->tdd_ctx) == 1) {
            result = BUT_SUCCESS;
        }
    }

    return result;
}


static but_result
test_count(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    but_result      result = BUT_FAIL;

    if (tdd->tdd_get_count(tdd->tdd_ctx) == 2) {
        result = BUT_SUCCESS;
    }

    return result;
}


static but_result
test_run(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    but_result      result = BUT_FAIL;

    tdd->tdd_run_current(tdd->tdd_ctx);
    tdd->tdd_next(tdd->tdd_ctx);
    tdd->tdd_run_current(tdd->tdd_ctx);

    if (tdd->tdd_get_count_passed(tdd->tdd_ctx) == 1
        && tdd->tdd_get_count_failed(tdd->tdd_ctx) == 1
        && tdd->tdd_get_count_failed_setup(tdd->tdd_ctx) == 0
        && tdd->tdd_get_count_results(tdd->tdd_ctx) == 1) {
        result = BUT_SUCCESS;
    }

    return result;
}


static but_result
test_results(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    but_context*    ctx = tdd->tdd_ctx;
    but_result      result = BUT_FAIL;

    tdd->tdd_run_current(ctx);
    tdd->tdd_next(ctx);
    tdd->tdd_run_current(ctx);

    // zero is a successful test, so we expect BUT_PASSED and BUT_FAILED
    if (tdd->tdd_get_result(ctx, 0) == BTR_PASSED
        && tdd->tdd_get_result(ctx, 1) == BTR_FAILED) {
        result = BUT_SUCCESS;
    }

    return result;
}
