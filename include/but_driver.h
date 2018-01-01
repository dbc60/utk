/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#ifndef BUT_DRIVER_H_INCLUDED
#define BUT_DRIVER_H_INCLUDED

#include "platform.h"
#include "but.h"


BEGIN_EXTERN_C

/** @brief validate the test context */
DllExport b32 but_is_valid(but_context *ctx);

/** @brief make a new test context for the given test suite */
DllExport but_context *but_new(utk_test_suite *bts);

/** @brief release a test context */
DllExport void but_delete(but_context *ctx);

/** @brief make the next test case the current one.
 * The most this will increment is to one past the last test case.
 */
DllExport void but_next(but_context *ctx);

/** return TRUE if there are more test cases that can be run.
 * If but_next has incremented beyond the last test case, then return
 * FALSE. Otherwise return TRUE.
 */
DllExport b32 but_more_test_cases(but_context *ctx);

/** @brief get the name of the test suite associated with the context */
DllExport const ch8 *but_get_name_test_suite(but_context *ctx);

/** @brief get the name of the current test case. */
DllExport const ch8 *but_get_name_test_case(but_context *ctx);

/** @brief get the zero-based index of the current test case */
DllExport size_t but_get_index(but_context *ctx);

/** @brief get the number of test cases in the test suite */
DllExport size_t but_get_count_test_cases(but_context *ctx);

/** @brief run the current test case */
DllExport void but_run(but_context *ctx);

/** @brief get the number of test cases run */
DllExport size_t but_get_count_run(but_context *ctx);

/** @brief get the number of test cases that passed */
DllExport size_t but_get_count_passed(but_context *ctx);

/** @brief get the number of test cases that failed */
DllExport size_t but_get_count_failed(but_context *ctx);

/** @brief get the number of test cases that failed in their setup function */
DllExport size_t but_get_count_failed_setup(but_context *ctx);

/** @brief get the number of test results */
DllExport size_t but_get_count_results(but_context *ctx);

/** @brief get the test result for a given test case by index */
DllExport but_test_result but_get_result(but_context *ctx, size_t index);

/** @brief get the error code for a given test case mapped by index */
DllExport int but_get_error_code(but_context *ctx, size_t index);

END_EXTERN_C

#endif  /* BUT_DRIVER_H_INCLUDED */
