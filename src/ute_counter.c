/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include "ute_counter.h"
#include <string.h>


// The test exception thrown by ute_throw_try(). It is public so the test
// driver can catch it.
ehm_exception exception_ute_test = {"UTE Test Exception"};


/**
 * Initialize a counter with a context.
 */
void
ute_counter_init(ute_counter *uc, ute_context *ctx)
{
    memset(uc, 0, sizeof *uc);
    uc->context = ctx;
}


/**
 * Retrieve a counter's context
 */
ute_context *
ute_counter_get_context(ute_counter *uc)
{
    return uc->context;
}

/** @brief Reset the exception point counter to the next exception point.
 * Enable throwing a test exception, and increment the fail count so the test
 * exception will be thrown if count_exception_point is incremented to the
 * value of count_fail in ute_throw_try(), below.
 */
void
ute_increment_count_fail(ute_counter *uc)
{
    uc->count_exception_point = 0;
    ++uc->count_fail;
    uc->enable_test_exception = TRUE;
}


/**
 * Increment the exception point. If it equals the fail count, then raise a
 * test exception.
 */
void
ute_throw_try(ute_counter *uc)
{
    if (uc->enable_test_exception
        && ++(uc->count_exception_point) == uc->count_fail) {
        EHM_THROW(exception_ute_test);
    }
}

/**
 * Return TRUE if the counter has thrown a test exception
 */
b32
ute_has_thrown(const ute_counter *uc)
{
    return uc->count_exception_point >= uc->count_fail;
}


/**
 * Disable throwing
 */
void
ute_throw_disable(ute_counter *uc)
{
    uc->enable_test_exception = FALSE;
}


/**
 * Enable throwing
 */
void
ute_throw_enable(ute_counter *uc)
{
    uc->enable_test_exception = TRUE;
}


/**
 * Return TRUE if throwing is enabled, false otherwise
 */
b32
ute_throw_is_enabled(const ute_counter *uc)
{
    return uc->enable_test_exception;
}


/**
 * Retrieve the current fail count
 */
size_t
ute_get_count_fail(ute_counter *uc)
{
    return uc->count_fail;
}


/**
 * Retrieve the current exception point
 */
size_t
ute_get_count_exception_point(ute_counter *uc)
{
    return uc->count_exception_point;
}


/**
 * Increment the fail count. Return TRUE if the fail count equals the current
 * exception point, and FALSE otherwise. The idea is to provide a non-throwing
 * interface to the counter.
 */
b32
ute_would_throw(ute_counter *uc) {
    b32 result = FALSE;

    if (uc->enable_test_exception
        && ++(uc->count_exception_point) == uc->count_fail) {
        result = TRUE;
    }

    return result;
}
