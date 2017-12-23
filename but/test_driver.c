/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include "test_driver.h"
#include <but_version.h>

// For NULL
#include <stddef.h>
// For strcmp()
#include <string.h>

// For HMODULE, LoadLibraryA and FreeLibrary
#include <windows.h>

#define DRIVER_LIBRARY      "but_driver.dll"
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

/**
 * The names of the methods (BDM = BUT Driver Method) exported
 * from but_driver.dll
 */
#define BDM_GET_VERSION_STR         "but_version"
#define BDM_GET_VERSION_NUM         "but_version_num"
#define BDM_IS_VALID                "but_is_valid"
#define BDM_NEW                     "but_new"
#define BDM_DELETE                  "but_delete"
#define BDM_NEXT                    "but_next"
#define BDM_MORE_CASES              "but_more_test_cases"
#define BDM_GET_SUITE_NAME          "but_get_name_test_suite"
#define BDM_GET_CASE_NAME           "but_get_name_test_case"
#define BDM_GET_CASE_INDEX          "but_get_index"
#define BDM_GET_COUNT               "but_get_count_test_cases"
#define BDM_RUN                     "but_run"
#define BDM_GET_PASS_COUNT          "but_get_count_passed"
#define BDM_GET_FAIL_COUNT          "but_get_count_failed"
#define BDM_GET_SETUP_FAIL_COUNT    "but_get_count_failed_setup"
#define BDM_GET_RESULTS_COUNT       "but_get_count_results"
#define BDM_GET_RESULT              "but_get_result"

// Function prototype typedefs
typedef const ch8 *(*get_version_string)(but_context*);
/* add two prototypes; one for getting the version as a 'ch8*', and the other
 * to get it as a 's32' for the test_version_valid test.
 */
typedef const ch8 * (*get_version_str)();
typedef s32 (*get_version_num)();
typedef int  (*is_valid)(but_context*);
typedef but_context * (*but_context_new)(but_test_suite *);
typedef void (*but_context_delete)(but_context*);
typedef void (*next)(but_context*);
typedef b32 (*more)(but_context*);
typedef const ch8 * (*get_name_case)(but_context*);
typedef const ch8 * (*get_name_suite)(but_context*);
typedef size_t (*get_index)(but_context* handle);
typedef size_t (*get_count)(but_context* handle);
typedef void (*run_current)(but_context* handle);
typedef size_t (*get_count_passed)(but_context*);
typedef size_t (*get_count_failed)(but_context*);
typedef size_t (*get_count_failed_setup)(but_context*);
typedef size_t (*get_count_results)(but_context*);
typedef but_test_result (*get_result)(but_context*, int);

// Test driver's data
typedef 
struct test_driver_data
{
    HMODULE                 tdd_handle;
    but_context            *tdd_ctx;
    but_test_suite         *tdd_ts;
    but_context_new         tdd_new;
    but_context_delete      tdd_delete;
    get_version_str         tdd_get_version_str;
    get_version_num         tdd_get_version_num;
    is_valid                tdd_is_valid;
    next                    tdd_next;
    more                    tdd_more;
    get_name_case           tdd_get_name_case;
    get_name_suite          tdd_get_name_suite;
    get_index               tdd_get_index;
    get_count               tdd_get_count;
    run_current             tdd_run_current;
    get_count_passed        tdd_get_count_passed;
    get_count_failed        tdd_get_count_failed;
    get_count_failed_setup  tdd_get_count_failed_setup;
    get_count_results       tdd_get_count_results;
    get_result              tdd_get_result;
} TestDriveData;

TestDriveData testData;

static but_result test_success(void *data);
static but_result test_case_2(void *data);

static but_result driver_setup(void *data);
static void driver_teardown(void *data);

static but_result context_setup(void *data);
static void context_teardown(void *data);

static but_result test_driver_load(void *data);
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


but_test_case test_case_driver_load =
{
    BTC_NAME_LOAD_DRIVER,
    NULL,
    &test_driver_load,
    NULL,
    NULL
};

but_test_case test_case_version_valid =
{
    BTC_NAME_VALIDATE_VERSION,
    &driver_setup,
    &test_version_valid,
    &driver_teardown,
    (void*)&testData
};

but_test_case test_case_new_delete =
{
    BTC_NAME_NEW_DELETE,
    &driver_setup,
    &test_new_delete,
    &driver_teardown,
    (void*)&testData
};

but_test_case test_case_is_valid_context =
{
    BTC_NAME_IS_VALID,
    driver_setup,
    test_is_valid,
    driver_teardown,
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


/**
 * A setup routine that loads the BUT driver DLL and resolves all
 * its exported symbols to fields in a TestDriveData struct. Those functions
 * will be exercised by various test cases.
 * 
 * Call driver_teardown to release the resouces allocated here
 */
static but_result
driver_setup(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    but_result      result = BUT_FAIL;

    tdd->tdd_handle = LoadLibraryA(DRIVER_LIBRARY);
    if (tdd->tdd_handle)
    {
        result = BUT_SUCCESS;
        tdd->tdd_get_version_str =
            (get_version_str)GetProcAddress(tdd->tdd_handle, 
                                            BDM_GET_VERSION_STR);
        result |= tdd->tdd_get_version_str == 0;
        
        tdd->tdd_get_version_num =
            (get_version_num)GetProcAddress(tdd->tdd_handle,
                                            BDM_GET_VERSION_NUM);
        result |= tdd->tdd_get_version_num == 0;
        
        tdd->tdd_is_valid =
            (is_valid)GetProcAddress(tdd->tdd_handle,
                                     BDM_IS_VALID);
        result |= tdd->tdd_is_valid == 0;

        tdd->tdd_new =
            (but_context_new)GetProcAddress(tdd->tdd_handle, BDM_NEW);
        result |= tdd->tdd_new == 0;

        tdd->tdd_delete =
            (but_context_delete)GetProcAddress(tdd->tdd_handle,
                                               BDM_DELETE);
        result |= tdd->tdd_delete == 0;

        tdd->tdd_next =
            (next)GetProcAddress(tdd->tdd_handle, BDM_NEXT);
        result |= tdd->tdd_next == 0;

        tdd->tdd_more =
            (more)GetProcAddress(tdd->tdd_handle, BDM_MORE_CASES);
        result |= tdd->tdd_more == 0;

        tdd->tdd_get_name_case =
            (get_name_case)GetProcAddress(tdd->tdd_handle,
                                          BDM_GET_CASE_NAME);
        result |= tdd->tdd_get_name_case == 0;

        tdd->tdd_get_name_suite =
            (get_name_suite)GetProcAddress(tdd->tdd_handle,
                                           BDM_GET_SUITE_NAME);
        result |= tdd->tdd_get_name_suite == 0;

        tdd->tdd_get_index =
            (get_index)GetProcAddress(tdd->tdd_handle,
                                      BDM_GET_CASE_INDEX);
        result |= tdd->tdd_get_index == 0;

        tdd->tdd_get_count =
            (get_count)GetProcAddress(tdd->tdd_handle, BDM_GET_COUNT);
        result |= tdd->tdd_get_count == 0;

        tdd->tdd_run_current =
            (run_current)GetProcAddress(tdd->tdd_handle, BDM_RUN);
        result |= tdd->tdd_run_current == 0;

        tdd->tdd_get_count_passed =
            (get_count_passed)GetProcAddress(tdd->tdd_handle,
                                             BDM_GET_PASS_COUNT);
        result |= tdd->tdd_get_count_passed == 0;

        tdd->tdd_get_count_failed =
            (get_count_failed)GetProcAddress(tdd->tdd_handle,
                                             BDM_GET_FAIL_COUNT);
        result |= tdd->tdd_get_count_failed == 0;

        tdd->tdd_get_count_failed_setup =
            (get_count_failed_setup)GetProcAddress(tdd->tdd_handle,
                                                   BDM_GET_SETUP_FAIL_COUNT);
        result |= tdd->tdd_get_count_failed_setup == 0;

        tdd->tdd_get_count_results =
            (get_count_results)GetProcAddress(tdd->tdd_handle,
                                              BDM_GET_RESULTS_COUNT);
        result |= tdd->tdd_get_count_results == 0;

        tdd->tdd_get_result =
            (get_result)GetProcAddress(tdd->tdd_handle, BDM_GET_RESULT);
        result |= tdd->tdd_get_result == 0;

        tdd->tdd_ts = &test_suite_success_fail;
    }

    return result;
}


static void
driver_teardown(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;

    if (tdd->tdd_handle) {
        FreeLibrary(tdd->tdd_handle);
        tdd->tdd_handle = 0;
    }
}


static but_result
context_setup(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    but_result result = 0;

    result = driver_setup(data);
    if (BUT_SUCCESS == result) {
        tdd->tdd_ctx = tdd->tdd_new(tdd->tdd_ts);
    
        if (!tdd->tdd_is_valid(tdd->tdd_ctx)) {
            driver_teardown(data);
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
    driver_teardown(data);
}


/**
 * @brief exported unit tests
 */

/* Just load and unload the test driver DLL */
static but_result
test_driver_load(void *data)
{
    HMODULE             library;
    but_result          result = BUT_FAIL;

    UNREFERENCED(data);
    library = LoadLibraryA(DRIVER_LIBRARY);

    if (library)
    {
        result = BUT_SUCCESS;
        FreeLibrary(library);
    }

    return result;
}


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
