#include <platform.h>
#include "but_test.h"
#include <string.h>

#define NULL_DATA_TEST_STR "Null Data"
#define TEST_DATA_TEST_STR "Test Data"
#define DRIVER_NAME_MSG_TEST_STR    "Driver Name Message"
#define TEST_RESULT_MSG_TEST_STR    "Test result Message"
#define TEST_PACKAGE_MSG_TEST_STR   "Test package Message"
#define NO_TEST_SUITE_MSG_TEST_STR  "No test suite Message"
#define FAILED_LOAD_MSG_TEST_STR    "Failed load test suite Message"
#define FAILED_TEST_CASE_MSG_STR    "Failed test case Message"
#define FAILED_TEST_SETUP_MSG_STR   "Failed test setup Message"
#define DOT_OUT_MSG_TEST_STR        "Dot out Message"
#define END_OF_LINE_MSG_TEST_STR    "End of line Message"
#define MSG_ID_TOO_SMALL_TEST_STR   "Out Of Range Message too small"
#define MSG_ID_TOO_LARGE_TEST_STR   "Out Of Range Message too large"

static int but_test_data_null(void *data);
static int but_test_data_test(void *data);

but_test_case tc1 = {NULL_DATA_TEST_STR, NULL, but_test_data_null, NULL, NULL};

#define TEST_DATA_STR "Data"
but_test_case tc2 = {
    TEST_DATA_TEST_STR, NULL, but_test_data_test, NULL, (void*)TEST_DATA_STR
};

static int
but_test_data_null(void *data)
{
    return (NULL != data);
}


static int
but_test_data_test(void *data)
{
    return ((NULL == data) || strcmp((char*)data, TEST_DATA_STR));
}
