/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include "test_ute_driver.h"
#include <ute_version.h>
#include <ute_driver.h>

// For NULL
#include <stddef.h>
// For strncmp()
#include <string.h>

// Internal test case for testing the driver
#define TC_NAME_SUCCESS    "Test Success"
#define TC_NAME_FAIL       "Test Fail"
#define SUITE_NAME          "Driver"

// Names of test cases
#define TC_NAME_VALIDATE_VERSION   "Validate Version"
#define TC_NAME_NEW_DELETE         "New and Delete Context"
#define TC_NAME_IS_VALID           "Validate Context"

ute_test_driver_data test_data;

static utk_result test_is_valid_version(void *data);
static utk_result test_new_delete(void *data);
static utk_result test_is_valid_context(void *data);
static utk_test_suite test_suite_success_fail;

utk_result
driver_setup(void *data) {
    ute_test_driver_data *tdd = (ute_test_driver_data*)data;

    tdd->tdd_get_version_str = &ute_get_version_str;
    tdd->tdd_get_version_num = &ute_get_version_num;
    tdd->tdd_new = &ute_new;
    tdd->tdd_delete = &ute_delete;
    tdd->tdd_is_valid = &ute_is_valid;
    tdd->tdd_ts = &test_suite_success_fail;

    return UTK_SUCCESS;
}


utk_test_case test_case_valid_version =
{
    TC_NAME_VALIDATE_VERSION,
    &driver_setup,
    &test_is_valid_version,
    NULL,
    (void*)&test_data
};

utk_test_case test_case_new_delete =
{
    TC_NAME_NEW_DELETE,
    &driver_setup,
    &test_new_delete,
    NULL,
    (void*)&test_data
};

utk_test_case test_case_valid_context =
{
    TC_NAME_IS_VALID,
    driver_setup,
    test_is_valid_context,
    NULL,
    (void*)&test_data
};

enum test_driver_results {
    TDR_SUCCESS = UTK_SUCCESS,
    TDR_EXPECTED_FAIL,
    TDR_CONTEXT_INVALID,
    TDR_VERSION_STR_NULL,
    TDR_VERSION_STR_INVALID,
    TDR_VERSION_NUM_INVALID,
    TDR_UNEXPECTED_NULL,
};

/**
 * @brief exported unit tests
 */

static utk_result
test_is_valid_version(void *data)
{
    utk_result result = TDR_SUCCESS;
    ute_test_driver_data *tdd = (ute_test_driver_data*)data;
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
    ute_test_driver_data * tdd = (ute_test_driver_data*)data;

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
    ute_test_driver_data * tdd = (ute_test_driver_data*)data;

    tdd->tdd_ctx = tdd->tdd_new(tdd->tdd_ts);
    if (NULL == tdd->tdd_ctx) {
        result = TDR_UNEXPECTED_NULL;
    } else if (!tdd->tdd_is_valid(tdd->tdd_ctx)) {
        result = TDR_CONTEXT_INVALID;
        tdd->tdd_delete(tdd->tdd_ctx);
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

static utk_test_case *tca[] =
{
    &test_case_success,
    &test_case_fail
};

static utk_test_suite test_suite_success_fail =
{
    SUITE_NAME,
    ARRAY_COUNT(tca),
    tca
};


