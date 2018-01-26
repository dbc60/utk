/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#pragma once

#include <but.h>
#include <ute_counter.h>

struct ute_counter_data
{
    ute_context    *ctx;
    ute_counter     ctr;
};
typedef struct ute_counter_data ute_counter_data;

extern utk_test_case test_case_counter_init;

// Test setup and teardown methods
utk_result setup_counter(void *data);
void teardown_counter(void *data);

// Test methods
extern utk_result test_initialization(void *data);
extern utk_result test_get_context(void *data);
extern utk_result test_increment_count_throw(void *data);
extern utk_result test_throw_try(void *data);
