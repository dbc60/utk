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


struct ute_counter
{
    /*
     * The number of the exception point that we are up to executing in the
     * code. It is initialized to zero (no exception points encounted, yet) and
     * incremented at each exception point. When its value reaches that of
     * cntThrowCount and if cntThrowTestException is TRUE, a test exception
     * will be thrown at the current exception point.
     */
    u64 count_exception_point;

    /*
     * The number of the exception point which should throw. It defaults to
     * zero (an invalid number for an exception point).
     */
    u64 count_throw;

    /*
     * TRUE if exceptions should be thrown when count_exception_point equals
     * throw_count.
     */
    b32 throw_test_exception;
    
    /*
     * This counts the number of times malloc, calloc, etc. have successfully
     * returned a non-null result, minus the number of times a valid pointer
     * has been passed to free/delete. This value should be zero when the test
     * is over.
     */
    u64 count_allocations;
    
    // the number of invalid pointers passed to free/delete
    u64 count_invalid_free;

    // The current test context
    ute_context * context;
};
typedef struct ute_counter ute_counter;

void ute_counter_init(ute_counter * uc, ute_context * ctx);
ute_context * ute_counter_get_context(ute_counter * uc);
void ute_throw_try(ute_counter * uc);
u64 ute_get_count_throw(ute_counter * uc);
void ute_increment_count_throw(ute_counter * uc);
void ute_throw_disable(ute_counter * uc);
void ute_throw_enable(ute_counter * uc);
b32 ute_throw_enabled(ute_counter * uc);
b32 ute_thrown(ute_counter * uc);
void ute_memory_allocated(ute_counter * uc, void * mem);
void ute_memory_released(ute_counter * uc, void * mem);
void ute_memory_invalid_free(ute_counter * uc, void * mem);
u64 ute_get_count_allocation(ute_counter * uc);
u64 ute_get_count_exception_point(ute_counter * uc);
