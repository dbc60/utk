/* ========================================================================
* (C) Copyright 2015,2017 Douglas B. Cuthbertson
*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the MIT license. See LICENSE for details.
* ========================================================================
*/

#include <utk.h>
#include <but.h>
#include <ehm.h>

// The name of the exported test suite
#define TS_NAME_EHM "EHM"

// The names of our test cases
#define TC_NAME_UNHANDLED "Unhandled Exception"
#define TC_NAME_CATCH_UNHANDLED "Catch Unhandled Exception"
#define TC_NAME_NESTED_CATCH "Nested Catch"
#define TC_NAME_CATCH "Catch"
#define TC_NAME_CATCH_ALL "Catch All"
#define TC_NAME_CATCH_FINALLY "Catch Finally"
#define TC_NAME_CATCH_ALL_FINALLY "Catch All Finally"

// Exceptions to catch or not to catch, as needed.
const ehm_exception exc_dont_catch = {"Catch Me If You Can!"};
const ehm_exception exc_catch_me = {"Catch Me"};


// Test methods
INTERNAL_FUNCTION utk_result test_unhandled_exception(void *data);
INTERNAL_FUNCTION utk_result test_catch_unhandled_exception(void *data);
INTERNAL_FUNCTION utk_result test_catch_nested(void *data);
INTERNAL_FUNCTION utk_result test_catch(void *data);
INTERNAL_FUNCTION utk_result test_catch_all(void *data);
INTERNAL_FUNCTION utk_result test_catch_finally(void *data);
INTERNAL_FUNCTION utk_result test_catch_all_finally(void *data);


/** @brief EHM test cases
 */

LOCAL_VARIABLE utk_test_case test_case_unhandled_exception =
{
    TC_NAME_UNHANDLED,
    NULL,
    &test_unhandled_exception,
    NULL,
    NULL
};

LOCAL_VARIABLE utk_test_case test_case_catch_unhandled_exception =
{
    TC_NAME_CATCH_UNHANDLED,
    NULL,
    &test_catch_unhandled_exception,
    NULL,
    NULL
};

LOCAL_VARIABLE utk_test_case test_case_catch_nested = 
{
    TC_NAME_NESTED_CATCH,
    NULL,
    &test_catch_nested,
    NULL,
    NULL
};

LOCAL_VARIABLE utk_test_case test_case_catch = 
{
    TC_NAME_CATCH,
    NULL,
    &test_catch,
    NULL,
    NULL
};

LOCAL_VARIABLE utk_test_case test_case_catch_all = 
{
    TC_NAME_CATCH_ALL,
    NULL,
    &test_catch_all,
    NULL,
    NULL
};

LOCAL_VARIABLE utk_test_case test_case_catch_finally = 
{
    TC_NAME_CATCH_FINALLY,
    NULL,
    &test_catch_finally,
    NULL,
    NULL
};

LOCAL_VARIABLE utk_test_case test_case_catch_all_finally = 
{
    TC_NAME_CATCH_ALL_FINALLY,
    NULL,
    &test_catch_all_finally,
    NULL,
    NULL
};


/**
 * @brief test case return values
 */
enum test_ehm_results {
    EHM_SUCCESS = UTK_SUCCESS,
    EHM_ABJECT_FAILURE,
    EHM_MISSED_CATCH,
    EHM_UNEXPECTED_CATCH,
    EHM_CATCH_BUT_NO_FINALLY,
    EHM_FINALLY_BUT_NO_CATCH
};

/**
 * @brief unit tests
 */

/** @brief Throw an unhandled exception.
 * This is really a test of the driver that is executing the test cases. While
 * it can verify the EHM module actually throws exceptions, its primary purpose
 * is to verify the test driver can catch a misbehaving test case. If the
 * driver doesn't catch this exception, then it will probably crash.
 */
static utk_result
test_unhandled_exception(void *data)
{
    utk_result result = EHM_ABJECT_FAILURE;
    UNREFERENCED(data);
    
    EHM_TRY {
        EHM_THROW(exc_dont_catch);
    } EHM_CATCH(exc_catch_me) {
        result = EHM_UNEXPECTED_CATCH;
    } EHM_ENDTRY;

    return result;
}

/**
 * Throw an exception and let it be caught by the outer catch block.
 */
static utk_result
test_catch_unhandled_exception(void *data) {
    utk_result result = EHM_SUCCESS;
    b32 finally = FALSE;
    b32 catch_all = FALSE;
    b32 wrong_catch = FALSE;

    UNREFERENCED(data);

    EHM_TRY {
        EHM_TRY {
            EHM_THROW(exc_dont_catch);
        } EHM_CATCH(exc_catch_me) {
            wrong_catch = TRUE;
        } EHM_ENDTRY;
    } EHM_CATCH_ALL {
        catch_all = TRUE;
    } EHM_FINALLY {
        finally = TRUE;
    } EHM_ENDTRY;

    if (wrong_catch) {
        result = EHM_UNEXPECTED_CATCH;
    } else if (catch_all) {
        if (!finally) {
            result = EHM_CATCH_BUT_NO_FINALLY;
        }
    } else if (finally) {
        result = EHM_FINALLY_BUT_NO_CATCH;
    } else {
        // no catch, no catch all, no finally
        result = EHM_ABJECT_FAILURE;
    }

    return result;
}

static utk_result
test_catch_nested(void *data)
{
    utk_result result = EHM_SUCCESS;
    b32 wrong_catch = FALSE;
    b32 nested_catch = FALSE;

    UNREFERENCED(data);

    EHM_TRY{
        EHM_TRY{
        EHM_THROW(exc_catch_me);
        } EHM_CATCH(exc_catch_me) {
            nested_catch = TRUE;
        } EHM_ENDTRY;
    } EHM_CATCH_ALL {
        wrong_catch = TRUE;
    } EHM_ENDTRY;

    if (wrong_catch) {
        result = EHM_UNEXPECTED_CATCH;
    } else if (!nested_catch) {
        result = EHM_MISSED_CATCH;
    }

    return result;
}

static utk_result
test_catch(void *data) {
    utk_result result = EHM_ABJECT_FAILURE;
    UNREFERENCED(data);

    EHM_TRY {
        EHM_THROW(exc_catch_me);
    } EHM_CATCH(exc_catch_me) {
        result = EHM_SUCCESS;
    } EHM_ENDTRY;

    return result;
}

static utk_result
test_catch_all(void *data) {
    utk_result result = EHM_ABJECT_FAILURE;
    UNREFERENCED(data);

    EHM_TRY {
        EHM_THROW(exc_catch_me);
    } EHM_CATCH_ALL {
        result = EHM_SUCCESS;
    } EHM_ENDTRY;

    return result;
}

static utk_result
test_catch_finally(void *data)
{
    utk_result result = EHM_ABJECT_FAILURE;
    b32 caught = FALSE;
    b32 finally = FALSE;

    UNREFERENCED(data);

    EHM_TRY {
        EHM_THROW(exc_catch_me);
    } EHM_CATCH(exc_catch_me) {
        caught = TRUE;
    } EHM_FINALLY {
        finally = TRUE;
    } EHM_ENDTRY;

    if (caught && finally) {
        result = EHM_SUCCESS;
    }

    return result;
}

static utk_result
test_catch_all_finally(void *data)
{
    utk_result result = EHM_ABJECT_FAILURE;
    b32 caught = FALSE;
    b32 finally = FALSE;

    UNREFERENCED(data);

    EHM_TRY {
        EHM_THROW(exc_catch_me);
    } EHM_CATCH_ALL {
        caught = TRUE;
    } EHM_FINALLY {
        finally = TRUE;
    } EHM_ENDTRY;

    if (caught && finally) {
        result = EHM_SUCCESS;
    }

    return result;
}


LOCAL_VARIABLE utk_test_case *tca[] =
{
    // EHM tests
    &test_case_unhandled_exception,
    &test_case_catch_unhandled_exception,
    &test_case_catch_nested,
    &test_case_catch,
    &test_case_catch_all,
    &test_case_catch_finally,
    &test_case_catch_all_finally
};

LOCAL_VARIABLE
utk_test_suite ts_ehm = {TS_NAME_EHM, 
                         ARRAY_COUNT(tca),
                         tca};

PROJECTAPI utk_test_suite *
test_suite_load(void)
{
    return &ts_ehm;
}
