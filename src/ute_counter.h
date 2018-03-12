/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#pragma once

#include <platform.h>
#include <ute.h>
#include <ehm.h>

EXTERN_C ehm_exception exception_ute_test;

struct ute_counter
{
    /*
     * The number of the exception point that we are up to executing in the
     * code. It is initialized to zero (no exception points encountered, yet)
     * and if enable_test_exception is TRUE, it is incremented at each
     * exception point. When its value reaches that of count_fail, a test
     * exception will be thrown.
     */
    size_t count_exception_point;

    /*
     * The number of the exception point which should cause a failure. It
     * defaults to zero (an invalid number for an exception point).
     */
    size_t count_fail;

    /*
     * TRUE if exceptions should be thrown when count_exception_point equals
     * throw_count.
     */
    b32 enable_test_exception;
    
    // The current test context
    ute_context *context;
};
typedef struct ute_counter ute_counter;

void ute_counter_init(ute_counter *uc, ute_context *ctx);
ute_context* ute_counter_get_context(ute_counter *uc);
void ute_throw_try(ute_counter *uc);
size_t ute_get_count_fail(ute_counter *uc);
void ute_increment_count_fail(ute_counter *uc);
void ute_throw_disable(ute_counter *uc);
void ute_throw_enable(ute_counter *uc);
b32 ute_throw_is_enabled(const ute_counter *uc);
b32 ute_has_thrown(const ute_counter *uc);
size_t ute_get_count_exception_point(ute_counter *uc);

/**
 * Increment the throw count. Return TRUE if the throw count equals the current
 * exception point, and FALSE otherwise. This provides an alternative,
 * non-throwing interface to ute_throw_try().
 */
b32 ute_would_throw(ute_counter *uc);
