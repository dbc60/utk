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

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
// SYSTEM_INFO
#include <Windows.h>


struct rgn_config
{
    u32  page_size;     // the system page size
    u32  page_scale;    // the exponent to apply to page_size to get page_size_scaled
    u32  page_size_scaled;   // page_size left shifted by page_scale
    u32  region_size;   // the system allocation granularity
    u32  region_scale;  // the exponent to apply to region_size to get region_size_scaled
    u32  region_size_scaled;    // mcRegionSize left shifted by mcRegionScale
    u32  alignment;     // usually 8 or 16 (must be a power of two no less than 8)
};

typedef struct rgn_config rgn_config;

// Test data
LOCAL_VARIABLE rgn_config rgn_cfg = {0};

// Test setup and teardown methods
INTERNAL_FUNCTION utk_result setup(void *data);

// The names of our test suite & test cases
#define TS_NAME_RGN_CONFIG "Region"
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
    RGN_SUCCESS = UTK_SUCCESS,
    RGN_CFG_INVALID_PAGE_SIZE,
    RGN_CFG_INVALID_PAGE_SCALE,
    RGN_CFG_INVALID_PAGE_SIZE_SCALED,
    RGN_CFG_INVALID_REGION_SIZE,
    RGN_CFG_INVALID_REGION_SCALE,
    RGN_CFG_INVALID_REGION_SIZE_SCALED,
    RGN_CFG_INVALID_ALIGNMENT,
    RGN_CFG_INVALID_LOCK,
    RGN_ABJECT_FAILURE
};


#define RGN_CFG_DEFAULT_PAGE_SCALE      1
#define RGN_CFG_DEFAULT_REGION_SCALE    0
#define RGN_CFG_DEFAULT_ALIGNMENT       8
#define RGN_CFG_ALIGNMENT_MAX           64

void
rgn_cfg_init(rgn_config* cfg) {
    SYSTEM_INFO sys_info;

    GetSystemInfo(&sys_info);
    cfg->page_size = sys_info.dwPageSize;
    cfg->page_scale = RGN_CFG_DEFAULT_PAGE_SCALE;
    cfg->page_size_scaled = sys_info.dwPageSize << RGN_CFG_DEFAULT_PAGE_SCALE;
    cfg->region_size = sys_info.dwAllocationGranularity;
    cfg->region_scale = RGN_CFG_DEFAULT_REGION_SCALE;
    cfg->region_size_scaled = sys_info.dwAllocationGranularity
                             << RGN_CFG_DEFAULT_REGION_SCALE;
    cfg->alignment = RGN_CFG_ALIGNMENT_MAX;
}

utk_result
setup(void *data)
{
    rgn_config* cfg = (rgn_config*)data;

    rgn_cfg_init(cfg);
    return RGN_SUCCESS;
}


utk_result test_cfg_init(void *data) {
    utk_result result = RGN_SUCCESS;
    rgn_config* cfg = (rgn_config*)data;
    SYSTEM_INFO sys_info;

    GetSystemInfo(&sys_info);
    if (cfg->page_size != sys_info.dwPageSize) {
        result = RGN_CFG_INVALID_PAGE_SIZE;
    } else if (cfg->page_scale != RGN_CFG_DEFAULT_PAGE_SCALE) {
        result = RGN_CFG_INVALID_PAGE_SCALE;
    } else if (cfg->page_size_scaled
               != sys_info.dwPageSize << RGN_CFG_DEFAULT_PAGE_SCALE) {
        RGN_CFG_INVALID_PAGE_SIZE_SCALED;
    } else if (cfg->region_size != sys_info.dwAllocationGranularity) {
        result = RGN_CFG_INVALID_REGION_SIZE;
    } else if (cfg->region_scale != RGN_CFG_DEFAULT_REGION_SCALE) {
        result = RGN_CFG_INVALID_REGION_SCALE;
    } else if (cfg->region_size_scaled != sys_info.dwAllocationGranularity
                                          << RGN_CFG_DEFAULT_REGION_SCALE) {
        result = RGN_CFG_INVALID_REGION_SIZE_SCALED;
    } else if (cfg->alignment != RGN_CFG_ALIGNMENT_MAX) {
        result = RGN_CFG_INVALID_ALIGNMENT;
    }

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
