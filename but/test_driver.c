#include "test_driver.h"

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
#define TEST_STR_VERIFY_VERSION     "Verify Version"
#define TEST_STR_LOAD_DRIVER        "Load Driver"
#define TEST_STR_BEGIN_END          "Begin and End"
#define TEST_STR_IS_VALID           "Is-valid"
#define TEST_STR_NEXT_INDEX_MORE    "Next/Index/More"
#define TEST_STR_CASE_NAME          "Case Name"
#define TEST_STR_SUITE_NAME         "Suite Name"
#define TEST_STR_INDEX              "Index"
#define TEST_STR_COUNT              "Count"
#define TEST_STR_RUN                "Run"
#define TEST_STR_RESULTS            "Results"

// GetProcAddress never uses wchar_t, so these are regular C-strings
#define CTX_STR_IS_VALID        "test_context_is_valid"
#define CTX_STR_BEGIN           "test_context_new"
#define CTX_STR_END             "test_context_delete"
#define CTX_STR_NEXT            "test_context_next"
#define CTX_STR_MORE_CASES      "test_context_more_test_cases"
#define CTX_STR_GET_SUITE_NAME  "test_context_get_name_test_suite"
#define CTX_STR_GET_CASE_NAME   "test_context_get_name_test_case"
#define CTX_STR_GET_CASE_INDEX  "test_context_get_index"
#define CTX_STR_GET_COUNT       "test_context_get_count_test_cases"
#define CTX_STR_RUN_CURRENT     "test_context_run"
#define CTX_STR_GET_PASS_COUNT  "test_context_get_count_passed"
#define CTX_STR_GET_FAIL_COUNT  "test_context_get_count_failed"
#define CTX_STR_GET_SETUP_FAIL_COUNT    "test_context_get_count_failed_setup"
#define CTX_STR_GET_RESULTS_COUNT       "test_context_get_count_results"
#define CTX_STR_GET_RESULT      "test_context_get_result"

// Function prototype typedefs
typedef const ch8 *(*get_version_string)(but_context*);
/* add two prototypes; one for getting the version as a 'ch8*', and the other
 * to get it as a 's32' for the testVersion test.
 */
typedef int  (*context_is_valid)(but_context*);
typedef but_context* (*context_new)(but_test_suite *);
typedef void (*context_delete)(but_context*);
typedef void (*context_next)(but_context*);
typedef b32 (*context_more)(but_context*);
typedef const ch8 *(*get_name_case)(but_context*);
typedef const ch8 *(*get_name_suite)(but_context*);
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
    HMODULE                 handle;
    but_context*           ctx;
    but_test_suite         *ts;
    context_new             context_new;
    context_delete          context_delete;
    context_is_valid        is_valid;
    context_next            next;
    context_more            more;
    get_name_case           get_name_case;
    get_name_suite          get_name_suite;
    get_index               get_index;
    get_count               get_count;
    run_current             run_current;
    get_count_passed        get_count_passed;
    get_count_failed        get_count_failed;
    get_count_failed_setup  get_count_failed_setup;
    get_count_results       get_count_results;
    get_result              get_result;
} TestDriveData;

TestDriveData testData;

static int test1(void *data);
static int test2(void *data);

static int loadDriver(void *data);
static int loadTestDriver(void *data);
static void unloadTestDriver(void *data);
static int setupContext(void *data);
static void teardownContext(void *data);
static int testVersion(void *data);
static int testBeginEnd(void *data);
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

but_test_case test_version = 
{
    TEST_STR_VERIFY_VERSION,
    NULL,
    &testVersion,
    NULL,
    NULL
};

but_test_case test_begin_end = 
{
    TEST_STR_BEGIN_END,
    &loadTestDriver,
    &testBeginEnd,
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

    tdd->handle = LoadLibraryA(DRIVER_LIBRARY);

    result = (tdd->handle) ? 0 : -1;

    if (result == 0)
    {
        tdd->is_valid = (context_is_valid)GetProcAddress(tdd->handle, CTX_STR_IS_VALID);
        result |= tdd->is_valid == 0;

        tdd->context_new = (context_new)GetProcAddress(tdd->handle, CTX_STR_BEGIN);
        result |= tdd->context_new == 0;

        tdd->context_delete = (context_delete)GetProcAddress(tdd->handle, CTX_STR_END);
        result |= tdd->context_delete == 0;

        tdd->next = (context_next)GetProcAddress(tdd->handle, CTX_STR_NEXT);
        result |= tdd->next == 0;

        tdd->more = (context_more)GetProcAddress(tdd->handle, CTX_STR_MORE_CASES);
        result |= tdd->more == 0;

        tdd->get_name_case =
            (get_name_case)GetProcAddress(tdd->handle, CTX_STR_GET_CASE_NAME);
        result |= tdd->get_name_case == 0;

        tdd->get_name_suite =
            (get_name_suite)GetProcAddress(tdd->handle, CTX_STR_GET_SUITE_NAME);
        result |= tdd->get_name_suite == 0;

        tdd->get_index =
            (get_index)GetProcAddress(tdd->handle, CTX_STR_GET_CASE_INDEX);
        result |= tdd->get_index == 0;

        tdd->get_count =
            (get_count)GetProcAddress(tdd->handle, CTX_STR_GET_COUNT);
        result |= tdd->get_count == 0;

        tdd->run_current =
            (run_current)GetProcAddress(tdd->handle, CTX_STR_RUN_CURRENT);
        result |= tdd->run_current == 0;

        tdd->get_count_passed =
            (get_count_passed)GetProcAddress(tdd->handle, CTX_STR_GET_PASS_COUNT);
        result |= tdd->get_count_passed == 0;

        tdd->get_count_failed =
            (get_count_failed)GetProcAddress(tdd->handle, CTX_STR_GET_FAIL_COUNT);
        result |= tdd->get_count_failed == 0;

        tdd->get_count_failed_setup =
            (get_count_failed_setup)GetProcAddress(tdd->handle, CTX_STR_GET_SETUP_FAIL_COUNT);
        result |= tdd->get_count_failed_setup == 0;

        tdd->get_count_results =
            (get_count_results)GetProcAddress(tdd->handle, CTX_STR_GET_RESULTS_COUNT);
        result |= tdd->get_count_results == 0;

        tdd->get_result =
            (get_result)GetProcAddress(tdd->handle, CTX_STR_GET_RESULT);
        result |= tdd->get_result == 0;

        tdd->ts = &ts;
    }

    return result;
}


static void
unloadTestDriver(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;

    if (tdd->handle) {
        FreeLibrary(tdd->handle);
        tdd->handle = 0;
    }
}   // unloadTestDriver


static int
setupContext(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    int result = 0;

    result = loadTestDriver(data);
    if (0 == result) {
        tdd->ctx = tdd->context_new(tdd->ts);
    
        if (!tdd->is_valid(tdd->ctx)) {
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

    tdd->context_delete(tdd->ctx);
    unloadTestDriver(data);
}


static int
testVersion(void *data)
{
    int result = -1;

    UNREFERENCED(data);

    return result;
}


static int
testBeginEnd(void *data)
{
    int                 result = 0;
    TestDriveData      *tdd = (TestDriveData*)data;

    tdd->ctx = tdd->context_new(tdd->ts);
    if (tdd->ctx) {
        tdd->context_delete(tdd->ctx);
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

    tdd->ctx = tdd->context_new(tdd->ts);
    if (tdd->ctx) {
        result = tdd->is_valid(tdd->ctx) ? 0 : -1;
        tdd->context_delete(tdd->ctx);
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

    if (tdd->get_count(tdd->ctx) > 0)
    {
        tdd->next(tdd->ctx);
        if (tdd->get_index(tdd->ctx) == 1) {
            if (tdd->more(tdd->ctx) == 1) {
                tdd->next(tdd->ctx);
                result = tdd->more(tdd->ctx);
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

    name = tdd->get_name_case(tdd->ctx);

    if (name) {
        result = strcmp(name, TEST_NAME1);
    } else {
        result = 1;
    }

    tdd->next(tdd->ctx);
    name = tdd->get_name_case(tdd->ctx);
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

    name = tdd->get_name_suite(tdd->ctx);

    result = strcmp(name, SUITE_NAME);

    tdd->next(tdd->ctx);
    name = tdd->get_name_suite(tdd->ctx);

    result |= strcmp(name, SUITE_NAME);

    return result;
}   // testSuiteName


static int
testIndex(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    int             result;

    result = tdd->get_index(tdd->ctx) != 0;
    tdd->next(tdd->ctx);
    result |= tdd->get_index(tdd->ctx) != 1;

    return result;
}   // testIndex


static int
testCount(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    int             result;

    result = tdd->get_count(tdd->ctx) != 2;
    return result;
}   // testCount


static int
testRun(void *data)
{
    TestDriveData  *tdd = (TestDriveData*)data;
    int             result = 0;

    tdd->run_current(tdd->ctx);
    tdd->next(tdd->ctx);
    tdd->run_current(tdd->ctx);

    result |= tdd->get_count_passed(tdd->ctx) != 1;
    result |= tdd->get_count_failed(tdd->ctx) != 1;
    result |= tdd->get_count_failed_setup(tdd->ctx) != 0;
    result |= tdd->get_count_results(tdd->ctx) != 1;

    return result;
}   // testRun


static int
testResults(void *data)
{
    TestDriveData      *tdd = (TestDriveData*)data;
    but_context*    ctx = tdd->ctx;
    int                 result = 0;

    tdd->run_current(ctx);
    tdd->next(ctx);
    tdd->run_current(ctx);

    // zero is a successful test, so we expect BUT_PASSED and BUT_FAILED
    result |= tdd->get_result(ctx, 0) != BTR_PASSED;
    result |= tdd->get_result(ctx, 1) != BTR_FAILED;

    return result;
}
