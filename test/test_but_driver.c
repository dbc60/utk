/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include "test_but_driver.h"
#include <but_driver.h>
#include <but_version.h>
#include <utk.h>

// For NULL
#include <stddef.h>
// For strncmp()
#include <string.h>

// The name of the exported test suite
#define TS_NAME_BUT "BUT"

#define BTR_STR "Data"


// Internal test case for testing the driver
#define TC_NAME_SUCCESS "Test Success"
#define TC_NAME_FAIL    "Test Fail"
#define SUITE_NAME      "Driver"

// Names of test cases
#define TC_NAME_NULL_DATA         "Null Data"
#define TC_NAME_STR_DATA          "String Data"
#define TC_NAME_VALIDATE_VERSION    "Validate Version"
#define TC_NAME_LOAD_DRIVER         "Load Driver"
#define TC_NAME_NEW_DELETE          "New and Delete Context"
#define TC_NAME_IS_VALID            "Validate Context"
#define TC_NAME_NEXT_INDEX_MORE     "Next/Index/More"
#define TC_NAME_CASE_NAME           "Case Name"
#define TC_NAME_SUITE_NAME          "Suite Name"
#define TC_NAME_INDEX               "Index"
#define TC_NAME_COUNT               "Count"
#define TC_NAME_RUN                 "Run"
#define TC_NAME_RESULTS             "Results"

but_test_driver_data test_data;

utk_result context_setup(void *data);
void context_teardown(void *data);

utk_result test_is_valid_version(void *data);
utk_result test_new_delete(void *data);
utk_result test_is_valid_context(void *data);
utk_result test_next_index_more(void *data);
utk_result test_name_case(void *data);
utk_result test_name_suite(void *data);
utk_result test_index(void *data);
utk_result test_count(void *data);
utk_result test_run(void *data);
utk_result test_results(void *data);
utk_test_suite test_suite_success_fail;


enum test_driver_results {
    TDR_SUCCESS = UTK_SUCCESS,
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
    TDR_UNEXPECTED_RESULT_2
};


utk_result
driver_setup(void *data) {
    but_test_driver_data  *tdd = (but_test_driver_data*)data;

    tdd->tdd_get_version_str = &but_get_version_str;
    tdd->tdd_get_version_num = &but_get_version_num;
    tdd->tdd_new = &but_new;
    tdd->tdd_delete = &but_delete;
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

    return TDR_SUCCESS;
}


/**
 * @brief a coupld of simple test cases for Basic Unit Test
 */
utk_result but_test_data_null(void *data);
utk_result but_test_data_str(void *data);

enum but_test_data_results {
    BTR_SUCCESS = UTK_SUCCESS,
    BTR_UNEXPECTED_NON_NULL,
    BTR_UNEXPECTED_NULL,
    BTR_INVALID
};

/**
* @brief Verify the utk_test_case "test_data" field  is the assigned string.
*/
utk_result
but_test_data_str(void *data)
{
    utk_result result = BTR_SUCCESS;

    if (NULL == data) {
        result = BTR_UNEXPECTED_NULL;
    } else if (0 != strncmp((char*)data, BTR_STR, sizeof (BTR_STR) - 1)) {
        result = BTR_INVALID;
    }

    return result;
}

utk_test_case test_case_null = {
    TC_NAME_NULL_DATA, NULL, but_test_data_null, NULL, NULL
};

utk_test_case test_case_str = {
    TC_NAME_STR_DATA, NULL, but_test_data_str, NULL, (void*)BTR_STR
};

/**
 * @brief Verify the utk_test_case "test_data" field  is null.
 */
utk_result
but_test_data_null(void *data)
{
    utk_result result = BTR_SUCCESS;

    if (data != NULL) {
        result = BTR_UNEXPECTED_NON_NULL;
    }

    return result;
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

utk_test_case test_case_next =
{
    TC_NAME_NEXT_INDEX_MORE,
    context_setup,
    test_next_index_more,
    context_teardown,
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

utk_test_case test_case_run =
{
    TC_NAME_RUN,
    context_setup,
    test_run,
    context_teardown,
    (void*)&test_data
};


utk_test_case test_case_results =
{
    TC_NAME_RESULTS,
    context_setup,
    test_results,
    context_teardown,
    (void*)&test_data
};


utk_result test_success(void *data) {
    UNREFERENCED(data);
    return TDR_SUCCESS;
}

utk_result test_fail(void *data) {
    UNREFERENCED(data);
    return TDR_EXPECTED_FAIL;
}

utk_test_case test_case_success =
{
    TC_NAME_SUCCESS,
    NULL,
    test_success,
    NULL,
    NULL
};

utk_test_case test_case_fail =
{
    TC_NAME_FAIL,
    NULL,
    test_fail,
    NULL,
    NULL
};

LOCAL_VARIABLE utk_test_case *tca_internal[] =
{
    &test_case_success,
    &test_case_fail
};

utk_test_suite test_suite_success_fail =
{
    SUITE_NAME,
    ARRAY_COUNT(tca_internal),
    tca_internal
};


utk_result
context_setup(void *data)
{
    but_test_driver_data *tdd = (but_test_driver_data*)data;
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


void
context_teardown(void *data)
{
    but_test_driver_data *tdd = (but_test_driver_data*)data;

    tdd->tdd_delete(tdd->tdd_ctx);
}


/**
 * @brief unit tests
 */

utk_result
test_is_valid_version(void *data)
{
    utk_result result = TDR_SUCCESS;
    but_test_driver_data *tdd = (but_test_driver_data*)data;
    const ch8 *version_str;
    s32 version_num;

    version_str = tdd->tdd_get_version_str();
    version_num = tdd->tdd_get_version_num();
    if (NULL == version_str) {
        result = TDR_VERSION_STR_NULL;
    } else if (0 != strncmp(version_str,
                            BUT_VERSION_STR,
                            ARRAY_COUNT(BUT_VERSION_STR) - 1)) {
        result = TDR_VERSION_STR_INVALID;
    } else if (version_num != BUT_VERSION_NUM) {
        result = TDR_VERSION_NUM_INVALID;
    }

    return result;
}


utk_result
test_new_delete(void *data)
{
    utk_result result = TDR_SUCCESS;
    but_test_driver_data * tdd = (but_test_driver_data*)data;

    tdd->tdd_ctx = tdd->tdd_new(tdd->tdd_ts);
    if (NULL == tdd->tdd_ctx) {
        result = TDR_UNEXPECTED_NULL;
    } else {
        tdd->tdd_delete(tdd->tdd_ctx);
    }

    return result;
}


utk_result
test_is_valid_context(void *data)
{
    utk_result result = TDR_SUCCESS;
    but_test_driver_data * tdd = (but_test_driver_data*)data;

    tdd->tdd_ctx = tdd->tdd_new(tdd->tdd_ts);
    if (NULL == tdd->tdd_ctx) {
        result = TDR_UNEXPECTED_NULL;
    } else if (!tdd->tdd_is_valid(tdd->tdd_ctx)) {
            result = TDR_CONTEXT_INVALID;
            tdd->tdd_delete(tdd->tdd_ctx);
    }

    return result;
}


/**
 * @todo - This is not a good test. It can fail in too many ways. If it doesn't
 * return UTK_SUCCESS, it will take more effort than necessary to figure out
 * why and where it failed.
 */
utk_result
test_next_index_more(void *data)
{
    but_test_driver_data  * tdd = (but_test_driver_data*)data;
    utk_result      result = TDR_SUCCESS;

    if (tdd->tdd_get_count(tdd->tdd_ctx) == 0) {
        result = TDR_UNEXPECTED_COUNT;
    } else {
        tdd->tdd_next(tdd->tdd_ctx);
        if (tdd->tdd_get_index(tdd->tdd_ctx) != 1) {
            result = TDR_UNEXPECTED_INDEX_1;
        } else if (tdd->tdd_more(tdd->tdd_ctx) != 1) {
            result = TDR_UNEXPECTED_END;
        } else {
            tdd->tdd_next(tdd->tdd_ctx);
            if (TRUE == tdd->tdd_more(tdd->tdd_ctx)) {
                result = TDR_UNEXPECTED_MORE;
            }
        }
    }

    return result;
}


utk_result
test_name_case(void *data)
{
    but_test_driver_data * tdd = (but_test_driver_data*)data;
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


utk_result
test_name_suite(void *data)
{
    but_test_driver_data * tdd = (but_test_driver_data*)data;
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


utk_result
test_index(void *data)
{
    but_test_driver_data * tdd = (but_test_driver_data*)data;
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


utk_result
test_count(void *data)
{
    but_test_driver_data  *tdd = (but_test_driver_data*)data;
    utk_result      result = TDR_SUCCESS;

    if (tdd->tdd_get_count(tdd->tdd_ctx) != 2) {
        result = TDR_UNEXPECTED_COUNT;
    }

    return result;
}


utk_result
test_run(void *data)
{
    but_test_driver_data  *tdd = (but_test_driver_data*)data;
    utk_result      result = TDR_SUCCESS;

    tdd->tdd_run_current(tdd->tdd_ctx);
    tdd->tdd_next(tdd->tdd_ctx);
    tdd->tdd_run_current(tdd->tdd_ctx);

    if (tdd->tdd_get_count_passed(tdd->tdd_ctx) != 1) {
        result = TDR_UNEXPECTED_COUNT_PASSED;
    } else if (tdd->tdd_get_count_failed(tdd->tdd_ctx) != 1) {
        result = TDR_UNEXPECTED_COUNT_FAILED;
    } else if (tdd->tdd_get_count_failed_setup(tdd->tdd_ctx) != 0) {
        result = TDR_UNEXPECTED_COUNT_FAILED_SETUP;
    } else if (tdd->tdd_get_count_results(tdd->tdd_ctx) != 1) {
        result = TDR_UNEXPECTED_COUNT_RESULTS;
    }

    return result;
}


utk_result
test_results(void *data)
{
    but_test_driver_data  *tdd = (but_test_driver_data*)data;
    but_context*    ctx = tdd->tdd_ctx;
    utk_result      result = TDR_SUCCESS;

    tdd->tdd_run_current(ctx);
    tdd->tdd_next(ctx);
    tdd->tdd_run_current(ctx);

    // We expect the first test to pass and the second to fail
    if (tdd->tdd_get_result(ctx, 0) != BTR_PASSED) {
        result = TDR_UNEXPECTED_RESULT_1;
    } else if (tdd->tdd_get_result(ctx, 1) != BTR_FAILED) {
        result = TDR_UNEXPECTED_RESULT_2;
    }

    return result;
}


LOCAL_VARIABLE utk_test_case *tca[] = 
{
    // basic BUT tests
    &test_case_null,
    &test_case_str,

    // BUT driver tests
    &test_case_valid_version,
    &test_case_new_delete,
    &test_case_valid_context,
    &test_case_next,
    &test_case_name_case,
    &test_case_name_suite,
    &test_case_index,
    &test_case_count,
    &test_case_run,
    &test_case_results
};

LOCAL_VARIABLE
utk_test_suite but_ts = {TS_NAME_BUT, 
                         ARRAY_COUNT(tca),
                         tca};

DLL_EXPORT utk_test_suite *
test_suite_load(void)
{
    return &but_ts;
}
