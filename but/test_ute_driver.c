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
#define TC_NAME_NEXT_INDEX_END     "Next/Index/End"
#define TC_NAME_CASE_NAME          "Case Name"
#define TC_NAME_SUITE_NAME         "Suite Name"
#define TC_NAME_INDEX              "Index"
#define TC_NAME_COUNT              "Count"

ute_test_driver_data test_data;

static utk_result context_setup(void *data);
static void context_teardown(void *data);

static utk_result test_is_valid_version(void *data);
static utk_result test_new_delete(void *data);
static utk_result test_is_valid_context(void *data);
static utk_result test_next_index_next_end(void *data);
static utk_result test_name_case(void *data);
static utk_result test_name_suite(void *data);
static utk_result test_index(void *data);
static utk_result test_count(void *data);

static utk_test_suite test_suite_success_fail;

utk_result
driver_setup(void *data) {
    ute_test_driver_data *tdd = (ute_test_driver_data*)data;

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

utk_test_case test_case_index_next_end =
{
    TC_NAME_NEXT_INDEX_END,
    driver_setup,
    test_next_index_next_end,
    NULL,
    (void*)&test_data
};

utk_test_case test_case_name_case =
{
    TC_NAME_CASE_NAME,
    context_setup,
    test_name_case,
    context_teardown,
    (void*)&test_data
};

utk_test_case test_case_name_suite =
{
    TC_NAME_SUITE_NAME,
    context_setup,
    test_name_suite,
    context_teardown,
    (void*)&test_data
};

utk_test_case test_case_index =
{
    TC_NAME_INDEX,
    context_setup,
    test_index,
    context_teardown,
    (void*)&test_data
};

utk_test_case test_case_count =
{
    TC_NAME_COUNT,
    context_setup,
    test_count,
    context_teardown,
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
    TDR_UNEXPECTED_COUNT,
    TDR_UNEXPECTED_INDEX_1,
    TDR_UNEXPECTED_INDEX_2,
    TDR_UNEXPECTED_END,
    TDR_UNEXPECTED_MORE,
    TDR_UNEXPECTED_NAME_1,
    TDR_UNEXPECTED_NAME_2,
    TDR_UNEXPECTED_RESULT_1,
    TDR_UNEXPECTED_RESULT_2
};


static utk_result
context_setup(void *data)
{
    ute_test_driver_data *tdd = (ute_test_driver_data*)data;
    utk_result result = 0;

    result = driver_setup(data);
    if (TDR_SUCCESS == result) {
        tdd->tdd_ctx = tdd->tdd_new(tdd->tdd_ts);
    
        if (!tdd->tdd_is_valid(tdd->tdd_ctx)) {
            result = TDR_CONTEXT_INVALID;
        }
    }

    return result;
}


static void
context_teardown(void *data)
{
    ute_test_driver_data *tdd = (ute_test_driver_data*)data;

    tdd->tdd_delete(tdd->tdd_ctx);
}


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

static utk_result
test_next_index_next_end(void *data)
{
    utk_result result = TDR_SUCCESS;
    ute_test_driver_data * tdd = (ute_test_driver_data*)data;

    if (tdd->tdd_get_count(tdd->tdd_ctx) == 0) {
        result = TDR_UNEXPECTED_COUNT;
    } else {
        tdd->tdd_next(tdd->tdd_ctx);
        if (tdd->tdd_get_index(tdd->tdd_ctx) != 1) {
            result = TDR_UNEXPECTED_INDEX_1;
        } else if (tdd->tdd_is_end(tdd->tdd_ctx)) {
            result = TDR_UNEXPECTED_END;
        } else {
            tdd->tdd_next(tdd->tdd_ctx);
            if (tdd->tdd_is_end(tdd->tdd_ctx) == FALSE) {
                result = TDR_UNEXPECTED_MORE;
            }
        }
    }

    return result;
}


static utk_result
test_name_case(void *data)
{
    ute_test_driver_data * tdd = (ute_test_driver_data*)data;
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
    ute_test_driver_data * tdd = (ute_test_driver_data*)data;
    utk_result result = TDR_SUCCESS;
    const ch8 *name;

    name = tdd->tdd_get_name_suite(tdd->tdd_ctx);
    if (NULL == name) {
        result = TDR_UNEXPECTED_NULL;
    } else if (strcmp(name, SUITE_NAME)) {
        result = TDR_UNEXPECTED_NAME_1;
    } else {
        tdd->tdd_next(tdd->tdd_ctx);
        name = tdd->tdd_get_name_suite(tdd->tdd_ctx);
        if (NULL == name) {
            result = TDR_UNEXPECTED_NULL;
        } else if (strcmp(name, SUITE_NAME)) {
            result = TDR_UNEXPECTED_NAME_2;
        }
    }

    return result;
}


static utk_result
test_index(void *data)
{
    ute_test_driver_data * tdd = (ute_test_driver_data*)data;
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


static utk_result
test_count(void *data)
{
    ute_test_driver_data  *tdd = (ute_test_driver_data*)data;
    utk_result      result = TDR_SUCCESS;

    if (tdd->tdd_get_count(tdd->tdd_ctx) != 2) {
        result = TDR_UNEXPECTED_COUNT;
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


