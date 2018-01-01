/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#pragma once

#include <but.h>
#include <ute_version.h>
#include <utk.h>
#include <ute.h>
//#include <ute_driver.h>

 // Function prototype typedefs
typedef const ch8* (*get_version_str)(void);
typedef u32 (*get_version_num)(void);
typedef ute_context * (*ute_context_new)(utk_test_suite *);
typedef void (*ute_context_delete)(ute_context*);
typedef b32 (*is_valid)(ute_context*);
//typedef void (*next)(ute_context*);
//typedef b32 (*more)(ute_context*);
//typedef const ch8 * (*get_name_case)(ute_context*);
//typedef const ch8 * (*get_name_suite)(ute_context*);
//typedef size_t (*get_index)(ute_context* handle);
//typedef size_t (*get_count)(ute_context* handle);
//typedef void (*run_current)(ute_context* handle);
//typedef size_t (*get_count_passed)(ute_context*);
//typedef size_t (*get_count_failed)(ute_context*);
//typedef size_t (*get_count_failed_setup)(ute_context*);
//typedef size_t (*get_count_results)(ute_context*);
//typedef ute_test_result (*get_result)(ute_context*, size_t);

// Test driver's data
struct ute_test_driver_data
{
    ute_context            *tdd_ctx;
    utk_test_suite         *tdd_ts;
    get_version_str         tdd_get_version_str;
    get_version_num         tdd_get_version_num;
    ute_context_new         tdd_new;
    ute_context_delete      tdd_delete;
    is_valid                tdd_is_valid;
//    next                    tdd_next;
//    more                    tdd_more;
//    get_name_case           tdd_get_name_case;
//    get_name_suite          tdd_get_name_suite;
//    get_index               tdd_get_index;
//    get_count               tdd_get_count;
//    run_current             tdd_run_current;
//    get_count_passed        tdd_get_count_passed;
//    get_count_failed        tdd_get_count_failed;
//    get_count_failed_setup  tdd_get_count_failed_setup;
//    get_count_results       tdd_get_count_results;
//    get_result              tdd_get_result;
};
typedef struct ute_test_driver_data ute_test_driver_data;

extern utk_test_case test_case_valid_version;
extern utk_test_case test_case_new_delete;
extern utk_test_case test_case_valid_context;
//extern utk_test_case test_case_driver_load;
//extern utk_test_case test_case_next;
//extern utk_test_case test_case_name_case;
//extern utk_test_case test_case_name_suite;
//extern utk_test_case test_case_index;
//extern utk_test_case test_case_count;
//extern utk_test_case test_case_run;
//extern utk_test_case test_case_results;

