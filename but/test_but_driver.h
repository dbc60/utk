/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#pragma once

#include <but.h>
#include <but_version.h>
#include <but_driver.h>

 // Function prototype typedefs
typedef const ch8* (*get_version_str)(void);
typedef u32 (*get_version_num)(void);
typedef b32 (*is_valid)(but_context*);
typedef but_context * (*but_context_new)(utk_test_suite *);
typedef void (*but_context_delete)(but_context*);
typedef void (*next)(but_context*);
typedef b32 (*more)(but_context*);
typedef const ch8 * (*get_name_case)(but_context*);
typedef const ch8 * (*get_name_suite)(but_context*);
typedef size_t (*get_index)(but_context* handle);
typedef size_t (*get_count)(but_context* handle);
typedef void (*run_current)(but_context* handle);
typedef size_t (*get_count_passed)(but_context*);
typedef size_t (*get_count_failed)(but_context*);
typedef size_t (*get_count_failed_setup)(but_context*);
typedef size_t (*get_count_results)(but_context*);
typedef but_test_result (*get_result)(but_context*, size_t);

// Test driver's data
struct but_test_driver_data
{
    but_context            *tdd_ctx;
    utk_test_suite         *tdd_ts;
    get_version_str         tdd_get_version_str;
    get_version_num         tdd_get_version_num;
    but_context_new         tdd_new;
    but_context_delete      tdd_delete;
    is_valid                tdd_is_valid;
    next                    tdd_next;
    more                    tdd_more;
    get_name_case           tdd_get_name_case;
    get_name_suite          tdd_get_name_suite;
    get_index               tdd_get_index;
    get_count               tdd_get_count;
    run_current             tdd_run_current;
    get_count_passed        tdd_get_count_passed;
    get_count_failed        tdd_get_count_failed;
    get_count_failed_setup  tdd_get_count_failed_setup;
    get_count_results       tdd_get_count_results;
    get_result              tdd_get_result;
};
typedef struct but_test_driver_data but_test_driver_data;
