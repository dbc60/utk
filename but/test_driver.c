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
#define TEST_NAME1          "Test 1"
#define TEST_NAME2          "Test 2"
#define SUITE_NAME          "Driver"

// Names of test cases
#define TEST_STR_VALIDATE_VERSION     "Verify Version"
#define TEST_STR_LOAD_DRIVER        "Load Driver"
#define TEST_STR_NEW_DELETE         "New and Delete Context"
#define TEST_STR_IS_VALID           "Is-valid"
#define TEST_STR_NEXT_INDEX_MORE    "Next/Index/More"
#define TEST_STR_CASE_NAME          "Case Name"
#define TEST_STR_SUITE_NAME         "Suite Name"
#define TEST_STR_INDEX              "Index"
#define TEST_STR_COUNT              "Count"
#define TEST_STR_RUN                "Run"
#define TEST_STR_RESULTS            "Results"

// The names of the functions exported from but_driver.dll
#define CTX_STR_GET_VERSION_STR         "but_version"
#define CTX_STR_GET_VERSION_NUM         "but_version_num"
#define CTX_STR_IS_VALID                "but_is_valid"
#define CTX_STR_NEW                     "but_new"
#define CTX_STR_DELETE                  "but_delete"
#define CTX_STR_NEXT                    "but_next"
#define CTX_STR_MORE_CASES              "but_more_test_cases"
#define CTX_STR_GET_SUITE_NAME          "but_get_name_test_suite"
#define CTX_STR_GET_CASE_NAME           "but_get_name_test_case"
#define CTX_STR_GET_CASE_INDEX          "but_get_index"
#define CTX_STR_GET_COUNT               "but_get_count_test_cases"
#define CTX_STR_RUN_CURRENT             "but_run"
#define CTX_STR_GET_PASS_COUNT          "but_get_count_passed"
#define CTX_STR_GET_FAIL_COUNT          "but_get_count_failed"
#define CTX_STR_GET_SETUP_FAIL_COUNT    "but_get_count_failed_setup"
#define CTX_STR_GET_RESULTS_COUNT       "but_get_count_results"
#define CTX_STR_GET_RESULT              "but_get_result"

// Function prototype typedefs
typedef const ch8 *(*get_version_string)(but_context*);
/* add two prototypes; one for getting the version as a 'ch8*', and the other
 * to get it as a 's32' for the testValidateVersion test.
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

static int test1(void *data);
static int test2(void *data);

static int loadDriver(void *data);
static int loadTestDriver(void *data);
static void unloadTestDriver(void *data);
static int setupContext(void *data);
static void teardownContext(void *data);
static int testValidateVersion(void *data);
static int testNewDelete(void *data);
static int testIsValid(void *data);
static int testNextIndexMore(void *data);
static int testCaseName(void *data);
static int testSuiteName(void *data);
static int testIndex(void *data);
static int testCount(void *data);
static int testRun(void *data);
static int testResults(void *data);


but_test_case load_driver =
{
    TEST_STR_LOAD_DRIVER,
    NULL,
    &loadDriver,
    NULL,
    NULL
};

but_test_case test_validate_version = 
{
    TEST_STR_VALIDATE_VERSION,
    &loadTestDriver,
    &testValidateVersion,
    &unloadTestDriver,
    (void*)&testData
};

but_test_case test_begin_end = 
{
    TEST_STR_NEW_DELETE,
    &loadTestDriver,
    &testNewDelete,
    &unloadTestDriver,
    (void*)&testData
};

but_test_case test_is_valid = 
{
    TEST_STR_IS_VALID,
    loadTestDriver,
    testIsValid,
    unloadTestDriver,
    (void*)&testData
};

but_test_case next_tc =
{
    TEST_STR_NEXT_INDEX_MORE,
    setupContext,
    testNextIndexMore,
    teardownContext,
    (void*)&testData
};

but_test_case case_name = 
{
    TEST_STR_CASE_NAME,
    setupContext,
    testCaseName,
    teardownContext,
    (void*)&testData
};

but_test_case suite_name = 
{
    TEST_STR_SUITE_NAME,
    setupContext,
    testSuiteName,
    teardownContext,
    (void*)&testData
};

but_test_case case_index = 
{
    TEST_STR_INDEX,
    setupContext,
    testIndex,
    teardownContext,
    (void*)&testData
};

but_test_case case_count = 
{
    TEST_STR_COUNT,
    setupContext,
    testCount,
    teardownContext,
    (void*)&testData
};

but_test_case test_run = 
{
    TEST_STR_RUN,
    setupContext,
    testRun,
    teardownContext,
    (void*)&testData
};


but_test_case test_results = 
{
    TEST_STR_RESULTS,
    setupContext,
    testResults,
    teardownContext,
    (void*)&testData
};


static int test1(void *data) {UNREFERENCED(data); return 0;}
static int test2(void *data) {UNREFERENCED(data); return -1;}

static but_test_case tc1 =
{
    TEST_NAME1,
    NULL,
    test1,
    NULL,
    NULL
};

static but_test_case tc2 = 
{
    TEST_NAME2,
    NULL,
    test2,
    NULL,
    NULL
};

static but_test_case *tca[] = 
{
    &tc1,
    &tc2
};

static but_test_suite ts = 
{
    SUITE_NAME,
    ARRAY_COUNT(tca),
    tca
};


static int
loadDriver(void *data)
{
    HMODULE             library;
    int                 result;

    UNREFERENCED(data);
    library = LoadLibraryA(DRIVER_LIBRARY);

    if (library)
    {
        result = 0;
        FreeLibrary(library);
    }
    else
    {
        result = -1;
    }
    return result;
}


static int
loadTestDriver(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    int             result;

    tdd->tdd_handle = LoadLibraryA(DRIVER_LIBRARY);

    result = (tdd->tdd_handle) ? 0 : -1;

    if (result == 0)
    {
        tdd->tdd_get_version_str = (get_version_str)GetProcAddress(tdd->tdd_handle, CTX_STR_GET_VERSION_STR);
        result |= tdd->tdd_get_version_str == 0;
        
        tdd->tdd_get_version_num = (get_version_num)GetProcAddress(tdd->tdd_handle, CTX_STR_GET_VERSION_NUM);
        result |= tdd->tdd_get_version_num == 0;
        
        tdd->tdd_is_valid = (is_valid)GetProcAddress(tdd->tdd_handle, CTX_STR_IS_VALID);
        result |= tdd->tdd_is_valid == 0;

        tdd->tdd_new = (but_context_new)GetProcAddress(tdd->tdd_handle, CTX_STR_NEW);
        result |= tdd->tdd_new == 0;

        tdd->tdd_delete = (but_context_delete)GetProcAddress(tdd->tdd_handle, CTX_STR_DELETE);
        result |= tdd->tdd_delete == 0;

        tdd->tdd_next = (next)GetProcAddress(tdd->tdd_handle, CTX_STR_NEXT);
        result |= tdd->tdd_next == 0;

        tdd->tdd_more = (more)GetProcAddress(tdd->tdd_handle, CTX_STR_MORE_CASES);
        result |= tdd->tdd_more == 0;

        tdd->tdd_get_name_case =
            (get_name_case)GetProcAddress(tdd->tdd_handle, CTX_STR_GET_CASE_NAME);
        result |= tdd->tdd_get_name_case == 0;

        tdd->tdd_get_name_suite =
            (get_name_suite)GetProcAddress(tdd->tdd_handle, CTX_STR_GET_SUITE_NAME);
        result |= tdd->tdd_get_name_suite == 0;

        tdd->tdd_get_index =
            (get_index)GetProcAddress(tdd->tdd_handle, CTX_STR_GET_CASE_INDEX);
        result |= tdd->tdd_get_index == 0;

        tdd->tdd_get_count =
            (get_count)GetProcAddress(tdd->tdd_handle, CTX_STR_GET_COUNT);
        result |= tdd->tdd_get_count == 0;

        tdd->tdd_run_current =
            (run_current)GetProcAddress(tdd->tdd_handle, CTX_STR_RUN_CURRENT);
        result |= tdd->tdd_run_current == 0;

        tdd->tdd_get_count_passed =
            (get_count_passed)GetProcAddress(tdd->tdd_handle, CTX_STR_GET_PASS_COUNT);
        result |= tdd->tdd_get_count_passed == 0;

        tdd->tdd_get_count_failed =
            (get_count_failed)GetProcAddress(tdd->tdd_handle, CTX_STR_GET_FAIL_COUNT);
        result |= tdd->tdd_get_count_failed == 0;

        tdd->tdd_get_count_failed_setup =
            (get_count_failed_setup)GetProcAddress(tdd->tdd_handle, CTX_STR_GET_SETUP_FAIL_COUNT);
        result |= tdd->tdd_get_count_failed_setup == 0;

        tdd->tdd_get_count_results =
            (get_count_results)GetProcAddress(tdd->tdd_handle, CTX_STR_GET_RESULTS_COUNT);
        result |= tdd->tdd_get_count_results == 0;

        tdd->tdd_get_result =
            (get_result)GetProcAddress(tdd->tdd_handle, CTX_STR_GET_RESULT);
        result |= tdd->tdd_get_result == 0;

        tdd->tdd_ts = &ts;
    }

    return result;
}


static void
unloadTestDriver(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;

    if (tdd->tdd_handle) {
        FreeLibrary(tdd->tdd_handle);
        tdd->tdd_handle = 0;
    }
}   // unloadTestDriver


static int
setupContext(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    int result = 0;

    result = loadTestDriver(data);
    if (0 == result) {
        tdd->tdd_ctx = tdd->tdd_new(tdd->tdd_ts);
    
        if (!tdd->tdd_is_valid(tdd->tdd_ctx)) {
            unloadTestDriver(data);
            result = -1;
        }
    }

    return result;
}


static void
teardownContext(void *data)
{
    TestDriveData *tdd = (TestDriveData*)data;

    tdd->tdd_delete(tdd->tdd_ctx);
    unloadTestDriver(data);
}


static int
testValidateVersion(void *data)
{
    int result = -1;
    TestDriveData *tdd = (TestDriveData*)data;
    const ch8 *version_str;
    s32 version_num;

    version_str = tdd->tdd_get_version_str();
    version_num = tdd->tdd_get_version_num();
    if (version_str) {
        if (0 == strncmp(version_str, BUT_VERSION, ARRAY_COUNT(BUT_VERSION) - 1)
            && BUT_VERSION_NUM == version_num) {
            result = 0;
        }
    }

    return result;
}


static int
testNewDelete(void *data)
{
    int                 result = 0;
    TestDriveData      *tdd = (TestDriveData*)data;

    tdd->tdd_ctx = tdd->tdd_new(tdd->tdd_ts);
    if (tdd->tdd_ctx) {
        tdd->tdd_delete(tdd->tdd_ctx);
    } else {
        result = -1;
    }

    return result;
}


static int
testIsValid(void *data)
{
    int                 result;
    TestDriveData      *tdd = (TestDriveData*)data;

    tdd->tdd_ctx = tdd->tdd_new(tdd->tdd_ts);
    if (tdd->tdd_ctx) {
        result = tdd->tdd_is_valid(tdd->tdd_ctx) ? 0 : -1;
        tdd->tdd_delete(tdd->tdd_ctx);
    }
    else
    {
        result = -1;
    }

    return result;
}


static int
testNextIndexMore(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    int             result = -1;

    if (tdd->tdd_get_count(tdd->tdd_ctx) > 0)
    {
        tdd->tdd_next(tdd->tdd_ctx);
        if (tdd->tdd_get_index(tdd->tdd_ctx) == 1) {
            if (tdd->tdd_more(tdd->tdd_ctx) == 1) {
                tdd->tdd_next(tdd->tdd_ctx);
                result = tdd->tdd_more(tdd->tdd_ctx);
            } else {
                result = 1;
            }
        } else {
            result = 2;
        }
    } else {
        result = 3;
    }

    return result;
}   // testNextIndexMore


static int
testCaseName(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    int             result;
    const ch8      *name;

    name = tdd->tdd_get_name_case(tdd->tdd_ctx);

    if (name) {
        result = strcmp(name, TEST_NAME1);
    } else {
        result = 1;
    }

    tdd->tdd_next(tdd->tdd_ctx);
    name = tdd->tdd_get_name_case(tdd->tdd_ctx);
    if (name) {
        result = strcmp(name, TEST_NAME2);
    } else {
        result = 2;
    }

    return result;
}   // testCaseName


static int
testSuiteName(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    int             result;
    const ch8      *name;

    name = tdd->tdd_get_name_suite(tdd->tdd_ctx);

    result = strcmp(name, SUITE_NAME);

    tdd->tdd_next(tdd->tdd_ctx);
    name = tdd->tdd_get_name_suite(tdd->tdd_ctx);

    result |= strcmp(name, SUITE_NAME);

    return result;
}   // testSuiteName


static int
testIndex(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    int             result;

    result = tdd->tdd_get_index(tdd->tdd_ctx) != 0;
    tdd->tdd_next(tdd->tdd_ctx);
    result |= tdd->tdd_get_index(tdd->tdd_ctx) != 1;

    return result;
}   // testIndex


static int
testCount(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    int             result;

    result = tdd->tdd_get_count(tdd->tdd_ctx) != 2;
    return result;
}   // testCount


static int
testRun(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    int             result = 0;

    tdd->tdd_run_current(tdd->tdd_ctx);
    tdd->tdd_next(tdd->tdd_ctx);
    tdd->tdd_run_current(tdd->tdd_ctx);

    result |= tdd->tdd_get_count_passed(tdd->tdd_ctx) != 1;
    result |= tdd->tdd_get_count_failed(tdd->tdd_ctx) != 1;
    result |= tdd->tdd_get_count_failed_setup(tdd->tdd_ctx) != 0;
    result |= tdd->tdd_get_count_results(tdd->tdd_ctx) != 1;

    return result;
}   // testRun


static int
testResults(void *data)
{
    TestDriveData      *tdd = (TestDriveData*)data;
    but_context*    ctx = tdd->tdd_ctx;
    int                 result = 0;

    tdd->tdd_run_current(ctx);
    tdd->tdd_next(ctx);
    tdd->tdd_run_current(ctx);

    // zero is a successful test, so we expect BUT_PASSED and BUT_FAILED
    result |= tdd->tdd_get_result(ctx, 0) != BTR_PASSED;
    result |= tdd->tdd_get_result(ctx, 1) != BTR_FAILED;

    return result;
}
