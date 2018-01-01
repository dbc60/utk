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

/** @brief make a new test context for the given test suite */
DllExport ute_context *ute_new(utk_test_suite *bts);

/** @brief release a test context */
DllExport void ute_delete(ute_context *ctx);

/** @brief validate the test context */
DllExport b32 ute_is_valid(ute_context *ctx);


END_EXTERN_C

#endif  /* UTE_DRIVER_H_INCLUDED */
