/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include <but.h>
#include "test_ehm.h"

#define TS_NAME_EHM "EHM"

LOCAL_VARIABLE utk_test_case *tca[] =
{
    // EHM tests
    &test_case_unhandled_exception,
    &test_case_catch_unhandled_exception,
    &test_case_catch,
    &test_case_catch_all,
    &test_case_catch_finally,
    &test_case_catch_all_finally
};

LOCAL_VARIABLE
utk_test_suite ts_ehm = {TS_NAME_EHM, 
                         ARRAY_COUNT(tca),
                         tca};

DLL_EXPORT utk_test_suite *
test_suite_load(void)
{
    return &ts_ehm;
}
