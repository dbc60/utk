/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include <but.h>
#include "but_test.h"
#include <string.h>

#define TEST_NAME_NULL_DATA  "Null Data"
#define TEST_NAME_STR_DATA   "String Data"

#define TEST_DATA_STR "Data"

INTERNAL_FUNCTION utk_result but_test_data_null(void *data);
INTERNAL_FUNCTION utk_result but_test_data_str(void *data);

utk_test_case test_case_null = {TEST_NAME_NULL_DATA, NULL, but_test_data_null, NULL, NULL};

utk_test_case test_case_str = {
    TEST_NAME_STR_DATA, NULL, but_test_data_str, NULL, (void*)TEST_DATA_STR
};


/**
 * @brief Verify the utk_test_case "test_data" field  is null.
 */
INTERNAL_FUNCTION utk_result
but_test_data_null(void *data)
{
    utk_result result = UTK_FAIL;

    if (NULL == data) {
        result = UTK_SUCCESS;
    }

    return result;
}


/**
* @brief Verify the utk_test_case "test_data" field  is the assigned string.
*/
INTERNAL_FUNCTION utk_result
but_test_data_str(void *data)
{
    utk_result result = UTK_FAIL;

    if (data != NULL &&
        0 == strncmp((char*)data, TEST_DATA_STR, sizeof (TEST_DATA_STR) - 1)) {
        result = UTK_SUCCESS;
    }

    return result;
}
