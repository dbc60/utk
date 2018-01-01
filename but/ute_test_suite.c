/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include <but.h>
#include "test_ute_driver.h"

#define UTE_TS_NAME "UTE"

LOCAL_VARIABLE utk_test_case *tca[] = 
{
    // UTE driver tests
    &test_case_valid_version,
    &test_case_new_delete,
    &test_case_valid_context
};

LOCAL_VARIABLE
utk_test_suite ute_ts = {UTE_TS_NAME, 
                         ARRAY_COUNT(tca),
                         tca};

DllExport utk_test_suite *
test_suite_load(void)
{
    return &ute_ts;
}
