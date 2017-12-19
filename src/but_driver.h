/* ========================================================================
   Author: Douglas B. Cuthbertson
   (C) Copyright 2015 by Douglas B. Cuthbertson. All Rights Reserved.
   ======================================================================== */

#ifndef BUT_DRIVER_H_INCLUDED
#define BUT_DRIVER_H_INCLUDED

#include "platform.h"
#include "but.h"


BEGIN_EXTERN_C

int test_context_is_valid(test_context *ctx);

/** @brief Test Context routines */

/** @brief make a new test context for the given test suite */
test_context *test_context_new(but_test_suite *bts);

/** @brief release a test context */
void test_context_delete(test_context *ctx);

/** @brief make the next test case the current one.
 * The most this will increment is to one past the last test case.
 */
void test_context_next(test_context *ctx);

/** return TRUE if there are more test cases that can be run.
 * If test_context_next has incremented beyond the last test case, then return
 * FALSE. Otherwise return TRUE.
 */
b32 test_context_more_test_cases(test_context *ctx);

/** @brief get the name of the test suite associated with the context */
const ch8 *test_context_get_name_test_suite(test_context *ctx);

/** @brief get the name of the current test case. */
const ch8 *test_context_get_name_test_case(test_context *ctx);

/** @brief get the zero-based index of the current test case */
size_t test_context_get_index(test_context *ctx);

/** @brief get the number of test cases in the test suite */
size_t test_context_get_count_test_cases(test_context *ctx);

/** @brief run the current test case */
void test_context_run(test_context *ctx);

/** @brief get the number of test cases run */
size_t test_context_get_count_run(test_context *ctx);

/** @brief get the number of test cases that passed */
size_t test_context_get_count_passed(test_context *ctx);

/** @brief get the number of test cases that failed */
size_t test_context_get_count_failed(test_context *ctx);

/** @brief get the number of test cases that failed in their setup function */
size_t test_context_get_count_failed_setup(test_context *ctx);

/** @brief get the number of test results */
size_t test_context_get_count_results(test_context *ctx);

/** @brief get the test result for a given test case by index */
but_test_result test_context_get_result(test_context *ctx, size_t index);

/** @brief get the error code for a given test case mapped by index */
int test_context_get_error_code(test_context *ctx, size_t index);

END_EXTERN_C

#endif  /* BUT_DRIVER_H_INCLUDED */
