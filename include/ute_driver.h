/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#ifndef UTE_DRIVER_H_INCLUDED
#define UTE_DRIVER_H_INCLUDED

#include "platform.h"
#include "ute.h"


BEGIN_EXTERN_C

/** @brief make a new test context for the given test suite.
 * Set the internal case iterator to the first test case.
 */
DllExport ute_context *ute_new(utk_test_suite *bts);

/** @brief release a test context */
DllExport void ute_delete(ute_context *ctx);

/** @brief validate the test context */
DllExport b32 ute_is_valid(ute_context *ctx);

/** @brief true if the case iterator is at the end of the set of test cases
 * in the suite */
DllExport b32 ute_is_end(ute_context *ctx);

/** @brief increment to the next test case, but no further than end */
DllExport void ute_next(ute_context *ctx);

/** @brief get the name of the test suite associated with the context */
DllExport const ch8 *ute_get_name_test_suite(ute_context *ctx);

/** @brief get the name of the current test case. */
DllExport const ch8 *ute_get_name_test_case(ute_context *ctx);

/** @brief get the zero-based index of the current test case */
DllExport size_t ute_get_index(ute_context *ctx);

/** @brief get the number of test cases in the test suite */
DllExport size_t ute_get_count_test_cases(ute_context *ctx);

/** @brief run the current test case */
DllExport void ute_run(ute_context *ctx);

END_EXTERN_C

#endif  /* UTE_DRIVER_H_INCLUDED */
