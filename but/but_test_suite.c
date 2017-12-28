/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include <but.h>
#include "test_driver.h"
#include "but_test.h"

#define BUT_TS_NAME "BUT"

static but_test_case *tca[] = 
{
    // basic BUT tests
    &test_case_null,
    &test_case_str,

    // BUT driver tests
    &test_case_valid_version,
    &test_case_new_delete,
    &test_case_valid_context,
    &test_case_next,
    &test_case_name_case,
    &test_case_name_suite,
    &test_case_index,
    &test_case_count,
    &test_case_run,
    &test_case_results
};

static
but_test_suite but_ts = {BUT_TS_NAME, 
                         ARRAY_COUNT(tca),
                         tca};

DllExport but_test_suite *
test_suite_load(void)
{
    return &but_ts;
}
