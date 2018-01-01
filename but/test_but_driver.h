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
    but_context            *btdd_ctx;
    utk_test_suite         *btdd_ts;
    get_version_str         btdd_get_version_str;
    get_version_num         btdd_get_version_num;
    but_context_new         btdd_new;
    but_context_delete      btdd_delete;
    is_valid                btdd_is_valid;
    next                    btdd_next;
    more                    btdd_more;
    get_name_case           btdd_get_name_case;
    get_name_suite          btdd_get_name_suite;
    get_index               btdd_get_index;
    get_count               btdd_get_count;
    run_current             btdd_run_current;
    get_count_passed        btdd_get_count_passed;
    get_count_failed        btdd_get_count_failed;
    get_count_failed_setup  btdd_get_count_failed_setup;
    get_count_results       btdd_get_count_results;
    get_result              btdd_get_result;
};
typedef struct but_test_driver_data but_test_driver_data;

extern utk_test_case test_case_driver_load;
extern utk_test_case test_case_valid_version;
extern utk_test_case test_case_new_delete;
extern utk_test_case test_case_valid_context;
extern utk_test_case test_case_next;
extern utk_test_case test_case_name_case;
extern utk_test_case test_case_name_suite;
extern utk_test_case test_case_index;
extern utk_test_case test_case_count;
extern utk_test_case test_case_run;
extern utk_test_case test_case_results;
