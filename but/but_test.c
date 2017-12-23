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

#define TEST_STR_NULL_DATA              "Null Data"
#define TEST_STR_TEST_DATA              "Test Data"

#define TEST_DATA_STR "Data"

static but_result but_test_data_null(void *data);
static but_result but_test_data_test(void *data);

but_test_case test_case_null = {TEST_STR_NULL_DATA, NULL, but_test_data_null, NULL, NULL};

but_test_case test_case_str = {
    TEST_STR_TEST_DATA, NULL, but_test_data_test, NULL, (void*)TEST_DATA_STR
};

static but_result
but_test_data_null(void *data)
{
    but_result result = BUT_FAIL;

    if (NULL == data) {
        result = BUT_SUCCESS;
    }

    return result;
}


static but_result
but_test_data_test(void *data)
{
    but_result result = BUT_FAIL;

    if (data != NULL && 0 == strcmp((char*)data, TEST_DATA_STR)) {
        result = BUT_SUCCESS;
    }

    return result;
}
