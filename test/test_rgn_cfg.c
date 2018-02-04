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
#include <utk_mutex.h>

struct rgn_config
{
    u32  page_size;     // the system page size
    u32  page_scale;    // the exponent to apply to page_size to get page_size_scaled
    u32  page_size_scaled;   // page_size left shifted by page_scale
    u32  region_size;   // the system allocation granularity
    u32  region_scale;  // the exponent to apply to region_size to get region_size_scaled
    u32  region_size_scaled;    // mcRegionSize left shifted by mcRegionScale
    u32  alignment;     // usually 8 or 16 (must be a power of two no less than 8)
    utk_mutex lock;     // lock/unlock to ensure thread-safety
};

typedef struct rgn_config rgn_config;

// Test data
LOCAL_VARIABLE rgn_config rgn_cfg = {0};

// Test setup and teardown methods
INTERNAL_FUNCTION utk_result setup(void *data);

// The names of our test suite & test cases
#define TS_NAME_RGN_CONFIG "Region Configuration"
#define TC_NAME_CFG_INIT "Initialization"

// Test methods
INTERNAL_FUNCTION utk_result test_cfg_init(void *data);

LOCAL_VARIABLE utk_test_case test_case_cfg_init =
{
    TC_NAME_CFG_INIT,
    setup, 
    &test_cfg_init, 
    NULL, 
    &rgn_cfg
};

enum test_results {
    RGN_CFG_SUCCESS = UTK_SUCCESS,
    RGN_CFG_ABJECT_FAILURE
};

utk_result
setup(void *data)
{
    utk_result result = RGN_CFG_ABJECT_FAILURE;
    //rgn_config* cfg = (rgn_config*)data;

    UNREFERENCED(data);

    return result;
}


utk_result test_cfg_init(void *data) {
    utk_result result = RGN_CFG_ABJECT_FAILURE;
    //rgn_config* cfg = (rgn_config*)data;

    UNREFERENCED(data);

    return result;
}


LOCAL_VARIABLE utk_test_case *tca[] =
{
    &test_case_cfg_init
};

LOCAL_VARIABLE
utk_test_suite ts_utk_mutex = {
    TS_NAME_RGN_CONFIG, 
    ARRAY_COUNT(tca),
    tca
};

PROJECTAPI utk_test_suite *
test_suite_load(void)
{
    return &ts_utk_mutex;
}
