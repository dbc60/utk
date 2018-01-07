/* ========================================================================
* (C) Copyright 2015,2017 Douglas B. Cuthbertson
*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the MIT license. See LICENSE for details.
* ========================================================================
*/

#include "test_ehm.h"

#define TC_NAME_UNHANDLED "Unhandled Exception"
#define TC_NAME_CATCH_UNHANDLED "Catch Unhandled Exception"
#define TC_NAME_CATCH "Catch"
#define TC_NAME_CATCH_ALL "Catch All"
#define TC_NAME_CATCH_FINALLY "Catch Finally"
#define TC_NAME_CATCH_ALL_FINALLY "Catch All Finally"

const ehm_exception exc_dont_catch = {"Catch Me If You Can!"};
const ehm_exception exc_catch_me = {"Catch Me"};

static utk_result test_unhandled_exception(void *data);
static utk_result test_catch_unhandled_exception(void *data);
static utk_result test_catch(void *data);
static utk_result test_catch_all(void *data);
static utk_result test_catch_finally(void *data);
static utk_result test_catch_all_finally(void *data);


/** @brief EHM test cases
 */

utk_test_case test_case_unhandled_exception =
{
    TC_NAME_UNHANDLED,
    NULL,
    &test_unhandled_exception,
    NULL,
    NULL
};

utk_test_case test_case_catch_unhandled_exception =
{
    TC_NAME_CATCH_UNHANDLED,
    NULL,
    &test_catch_unhandled_exception,
    NULL,
    NULL
};

utk_test_case test_case_catch = 
{
    TC_NAME_CATCH,
    NULL,
    &test_catch,
    NULL,
    NULL
};

utk_test_case test_case_catch_all = 
{
    TC_NAME_CATCH_ALL,
    NULL,
    &test_catch_all,
    NULL,
    NULL
};

utk_test_case test_case_catch_finally = 
{
    TC_NAME_CATCH_FINALLY,
    NULL,
    &test_catch_finally,
    NULL,
    NULL
};

utk_test_case test_case_catch_all_finally = 
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
    EHM_UNEXPECTED_CATCH,
    EHM_CATCH_BUT_NO_FINALLY,
    EHM_FINALLY_BUT_NO_CATCH
};

/**
* @brief unit tests
*/

/** @brief Throw an unhandled exception.
 * The test driver will have to catch it and display a nice error message.
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
