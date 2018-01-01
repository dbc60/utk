/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include "test_ute_driver.h"
#include <ute_version.h>
//#include <ute_driver.h>

// For NULL
#include <stddef.h>
// For strncmp()
#include <string.h>

// Names of test cases
#define BTC_NAME_VALIDATE_VERSION   "Validate Version"

ute_test_driver_data test_data;

static utk_result test_is_valid_version(void *data);

utk_result
driver_setup(void *data) {
    ute_test_driver_data *utdd = (ute_test_driver_data*)data;

    utdd->utdd_get_version_str = &ute_get_version_str;
    utdd->utdd_get_version_num = &ute_get_version_num;

    return UTK_SUCCESS;
}


utk_test_case test_case_valid_version =
{
    BTC_NAME_VALIDATE_VERSION,
    &driver_setup,
    &test_is_valid_version,
    NULL,
    (void*)&test_data
};

/**
 * @brief exported unit tests
 */

static utk_result
test_is_valid_version(void *data)
{
    utk_result result = UTK_FAIL;
    ute_test_driver_data *tdd = (ute_test_driver_data*)data;
    const ch8 *version_str;
    s32 version_num;

    version_str = tdd->utdd_get_version_str();
    version_num = tdd->utdd_get_version_num();
    if (version_str) {
        if (0 == strncmp(version_str,
                         UTE_VERSION_STR,
                         ARRAY_COUNT(UTE_VERSION_STR) - 1)
            && UTE_VERSION_NUM == version_num) {
            result = UTK_SUCCESS;
        }
    }

    return result;
}
