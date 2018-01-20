/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include "ute_counter.h"
#include <string.h>


ehm_exception exception_ute_test = {"UTE Test Exception"};


void
ute_counter_init(ute_counter *uc, ute_context *ctx)
{
    memset(uc, 0, sizeof *uc);
    uc->context = ctx;
}

ute_context *
ute_counter_get_context(ute_counter *uc)
{
    return uc->context;
}

/** @brief update the throw count to the next exception point
 * Reset the exception point counter, enable throwing a test exception, and
 * increment the throw count so the test exception will be thrown if an
 * exception point equal to the throw count is reached.
 */
void
ute_increment_count_throw(ute_counter *uc)
{
    uc->count_exception_point = 0;
    ++uc->count_throw;
    uc->throw_test_exception = TRUE;
}

u64
ute_get_count_allocation(ute_counter *uc)
{
    return uc->count_allocations;
}

void
ute_throw_try(ute_counter *uc)
{
    if (uc->throw_test_exception
        && ++(uc->count_exception_point) == uc->count_throw) {
        EHM_THROW(exception_ute_test);
    }
}

b32
ute_thrown(const ute_counter *uc)
{
    return uc->count_exception_point >= uc->count_throw;
}

void
ute_throw_disable(ute_counter *uc)
{
    uc->throw_test_exception = FALSE;
}

void
ute_throw_enable(ute_counter *uc)
{
    uc->throw_test_exception = TRUE;
}

b32
ute_throw_is_enabled(const ute_counter *uc)
{
    return uc->throw_test_exception;
}

u64
ute_get_count_throw(ute_counter *uc)
{
    return uc->count_throw;
}

u64
ute_get_count_exception_point(ute_counter *uc)
{
    return uc->count_exception_point;
}
