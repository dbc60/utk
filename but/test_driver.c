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
#define LOAD_DRIVER_TEST_STR        "Load Driver"
#define BEGIN_END_TEST_STR          "Begin and End"
#define IS_VALID_TEST_STR           "Is-valid"
#define NEXT_INDEX_MORE_TEST_STR    "Next/Index/More"
#define CASE_NAME_TEST_STR          "Case Name"
#define SUITE_NAME_TEST_STR         "Suite Name"
#define INDEX_TEST_STR              "Index"
#define COUNT_TEST_STR              "Count"
#define RUN_TEST_STR                "Run"
#define RESULTS_TEST_STR            "Results"

// GetProcAddress never uses wchar_t, so these are regular C-strings
#define IS_VALID_CTX_STR        "test_context_is_valid"
#define BEGIN_CTX_STR           "test_context_new"
#define END_CTX_STR             "test_context_delete"
#define NEXT_CTX_STR            "test_context_next"
#define MORE_CASES_CTX_STR      "test_context_more_test_cases"
#define GET_SUITE_NAME_CTX_STR  "test_context_get_name_test_suite"
#define GET_CASE_NAME_CTX_STR   "test_context_get_name_test_case"
#define GET_CASE_INDEX_CTX_STR  "test_context_get_index"
#define GET_COUNT_CTX_STR       "test_context_get_count_test_cases"
#define RUN_CURRENT_CTX_STR     "test_context_run"
#define GET_PASS_COUNT_CTX_STR  "test_context_get_count_passed"
#define GET_FAIL_COUNT_CTX_STR  "test_context_get_count_failed"
#define GET_SETUP_FAIL_COUNT_CTX_STR    "test_context_get_count_failed_setup"
#define GET_RESULTS_COUNT_CTX_STR       "test_context_get_count_results"
#define GET_RESULT_CTX_STR      "test_context_get_result"

// Function prototype typedefs
typedef int  (*context_is_valid)(test_context*);
typedef test_context* (*context_new)(but_test_suite *);
typedef void (*context_delete)(test_context*);
typedef void (*context_next)(test_context*);
typedef b32 (*context_more)(test_context*);
typedef const ch8 *(*get_name_case)(test_context*);
typedef const ch8 *(*get_name_suite)(test_context*);
typedef size_t (*get_index)(test_context* handle);
typedef size_t (*get_count)(test_context* handle);
typedef void (*run_current)(test_context* handle);
typedef size_t (*get_count_passed)(test_context*);
typedef size_t (*get_count_failed)(test_context*);
typedef size_t (*get_count_failed_setup)(test_context*);
typedef size_t (*get_count_results)(test_context*);
typedef but_test_result (*get_result)(test_context*, int);

// Test driver's data
typedef 
struct test_driver_data
{
    HMODULE                 handle;
    test_context*           ctx;
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
    LOAD_DRIVER_TEST_STR,
    NULL,
    &loadDriver,
    NULL,
    NULL
};

but_test_case test_begin_end = 
{
    BEGIN_END_TEST_STR,
    &loadTestDriver,
    &testBeginEnd,
    &unloadTestDriver,
    (void*)&testData
};

but_test_case test_is_valid = 
{
    IS_VALID_TEST_STR,
    loadTestDriver,
    testIsValid,
    unloadTestDriver,
    (void*)&testData
};

but_test_case next_tc =
{
    NEXT_INDEX_MORE_TEST_STR,
    setupContext,
    testNextIndexMore,
    teardownContext,
    (void*)&testData
};

but_test_case case_name = 
{
    CASE_NAME_TEST_STR,
    setupContext,
    testCaseName,
    teardownContext,
    (void*)&testData
};

but_test_case suite_name = 
{
    SUITE_NAME_TEST_STR,
    setupContext,
    testSuiteName,
    teardownContext,
    (void*)&testData
};

but_test_case case_index = 
{
    INDEX_TEST_STR,
    setupContext,
    testIndex,
    teardownContext,
    (void*)&testData
};

but_test_case case_count = 
{
    COUNT_TEST_STR,
    setupContext,
    testCount,
    teardownContext,
    (void*)&testData
};

but_test_case test_run = 
{
    RUN_TEST_STR,
    setupContext,
    testRun,
    teardownContext,
    (void*)&testData
};


but_test_case test_results = 
{
    RESULTS_TEST_STR,
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
        tdd->is_valid = (context_is_valid)GetProcAddress(tdd->handle, IS_VALID_CTX_STR);
        result |= tdd->is_valid == 0;

        tdd->context_new = (context_new)GetProcAddress(tdd->handle, BEGIN_CTX_STR);
        result |= tdd->context_new == 0;

        tdd->context_delete = (context_delete)GetProcAddress(tdd->handle, END_CTX_STR);
        result |= tdd->context_delete == 0;

        tdd->next = (context_next)GetProcAddress(tdd->handle, NEXT_CTX_STR);
        result |= tdd->next == 0;

        tdd->more = (context_more)GetProcAddress(tdd->handle, MORE_CASES_CTX_STR);
        result |= tdd->more == 0;

        tdd->get_name_case =
            (get_name_case)GetProcAddress(tdd->handle, GET_CASE_NAME_CTX_STR);
        result |= tdd->get_name_case == 0;

        tdd->get_name_suite =
            (get_name_suite)GetProcAddress(tdd->handle, GET_SUITE_NAME_CTX_STR);
        result |= tdd->get_name_suite == 0;

        tdd->get_index =
            (get_index)GetProcAddress(tdd->handle, GET_CASE_INDEX_CTX_STR);
        result |= tdd->get_index == 0;

        tdd->get_count =
            (get_count)GetProcAddress(tdd->handle, GET_COUNT_CTX_STR);
        result |= tdd->get_count == 0;

        tdd->run_current =
            (run_current)GetProcAddress(tdd->handle, RUN_CURRENT_CTX_STR);
        result |= tdd->run_current == 0;

        tdd->get_count_passed =
            (get_count_passed)GetProcAddress(tdd->handle, GET_PASS_COUNT_CTX_STR);
        result |= tdd->get_count_passed == 0;

        tdd->get_count_failed =
            (get_count_failed)GetProcAddress(tdd->handle, GET_FAIL_COUNT_CTX_STR);
        result |= tdd->get_count_failed == 0;

        tdd->get_count_failed_setup =
            (get_count_failed_setup)GetProcAddress(tdd->handle, GET_SETUP_FAIL_COUNT_CTX_STR);
        result |= tdd->get_count_failed_setup == 0;

        tdd->get_count_results =
            (get_count_results)GetProcAddress(tdd->handle, GET_RESULTS_COUNT_CTX_STR);
        result |= tdd->get_count_results == 0;

        tdd->get_result =
            (get_result)GetProcAddress(tdd->handle, GET_RESULT_CTX_STR);
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
    test_context*    ctx = tdd->ctx;
    int                 result = 0;

    tdd->run_current(ctx);
    tdd->next(ctx);
    tdd->run_current(ctx);

    // zero is a successful test, so we expect BUT_PASSED and BUT_FAILED
    result |= tdd->get_result(ctx, 0) != BTR_PASSED;
    result |= tdd->get_result(ctx, 1) != BTR_FAILED;

    return result;
}
