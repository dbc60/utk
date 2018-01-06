/* ========================================================================
* (C) Copyright 2015,2017 Douglas B. Cuthbertson
*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the MIT license. See LICENSE for details.
* ========================================================================
*/

#include "test_ehm.h"

#define TC_NAME_CATCH_UNHANDLED "Catch Unhandled Exception"

const ehm_exception exc_dont_catch = {"Don't Catch"};
const ehm_exception exc_catch_me = {"Catch Me"};

static utk_result test_catch_unhandled_exception(void *data);

utk_test_case test_case_catch_unhandled_exception =
{
    TC_NAME_CATCH_UNHANDLED,
    NULL,
    &test_catch_unhandled_exception,
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
    EHM_FINALL_BUT_NO_CATCH
};

/**
* @brief unit tests
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
        result = EHM_FINALL_BUT_NO_CATCH;
    } else {
        // no catch, no catch all, no finally
        result = EHM_ABJECT_FAILURE;
    }

    return result;
}
