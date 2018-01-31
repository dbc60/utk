/* ========================================================================
* (C) Copyright 2015,2017 Douglas B. Cuthbertson
*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the MIT license. See LICENSE for details.
* ========================================================================
*/

#include <platform.h>
#include <utk.h>
#include <but.h>
#include "test_utk_mutex.h"

// The name of the exported test suite
#define TS_NAME_UTK_MUTEX "UTK_MUTEX"

// The names of our test cases
#define TC_NAME_LOCK        "Lock"
#define TC_NAME_UNLOCK      "Unlock"
#define TC_NAME_TRYLOCK     "Trylock"

// Test methods
INTERNAL_FUNCTION utk_result test_lock(void *data);
INTERNAL_FUNCTION utk_result test_unlock(void *data);
INTERNAL_FUNCTION utk_result test_trylock(void *data);

/** @brief mutex test cases
 */

utk_test_case test_case_lock =
{
    TC_NAME_LOCK,
    NULL,
    &test_lock,
    NULL,
    NULL
};

utk_test_case test_case_unlock =
{
    TC_NAME_UNLOCK,
    NULL,
    &test_unlock,
    NULL,
    NULL
};

utk_test_case test_case_trylock =
{
    TC_NAME_TRYLOCK,
    NULL,
    &test_trylock,
    NULL,
    NULL
};


/**
 * @brief test case return values
 */
enum test_mutex_results {
    MTX_SUCCESS = UTK_SUCCESS,
    MTX_ABJECT_FAILURE
};


/**
 * @brief unit tests
 */

utk_result test_lock(void *data) {
    UNREFERENCED(data);

    return MTX_ABJECT_FAILURE;
}

utk_result test_unlock(void *data) {
    UNREFERENCED(data);

    return MTX_ABJECT_FAILURE;
}

utk_result test_trylock(void *data) {
    UNREFERENCED(data);

    return MTX_ABJECT_FAILURE;
}



LOCAL_VARIABLE utk_test_case *tca[] =
{
    &test_case_lock,
    &test_case_unlock,
    &test_case_trylock
};

LOCAL_VARIABLE
utk_test_suite ts_utk_mutex = {
    TS_NAME_UTK_MUTEX, 
    ARRAY_COUNT(tca),
    tca
};

DLL_EXPORT utk_test_suite *
test_suite_load(void)
{
    return &ts_utk_mutex;
}
