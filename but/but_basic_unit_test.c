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
    &tc1,
    &tc2,

    &load_driver,
    &test_validate_version,
    &test_begin_end,
    &test_is_valid,
    &next_tc,
    &case_name,
    &suite_name,
    &case_index,
    &case_count,
    &test_run,
    &test_results
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
