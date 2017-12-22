#include <platform.h>
#include <but.h>
#include "but_test.h"
#include <string.h>

#define TEST_STR_NULL_DATA              "Null Data"
#define TEST_STR_TEST_DATA              "Test Data"
#define TEST_STR_DRIVER_NAME_MSG        "Driver Name Message"
#define TEST_STR_TEST_RESULT_MSG        "Test result Message"
#define TEST_STR_TEST_PACKAGE_MSG       "Test package Message"
#define TEST_STR_NO_TEST_SUITE_MSG      "No test suite Message"
#define TEST_STR_FAILED_LOAD_MSG        "Failed load test suite Message"
#define TEST_STR_FAILED_TEST_CASE_MSG   "Failed test case Message"
#define TEST_STR_FAILED_TEST_SETUP_MSG  "Failed test setup Message"
#define TEST_STR_DOT_OUT_MSG            "Dot out Message"
#define TEST_STR_END_OF_LINE_MSG        "End of line Message"
#define TEST_STR_MSG_ID_TOO_SMALL       "Out Of Range Message too small"
#define TEST_STR_MSG_ID_TOO_LARGE       "Out Of Range Message too large"

static but_result but_test_data_null(void *data);
static but_result but_test_data_test(void *data);

but_test_case tc1 = {TEST_STR_NULL_DATA, NULL, but_test_data_null, NULL, NULL};

#define TEST_DATA_STR "Data"
but_test_case tc2 = {
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
