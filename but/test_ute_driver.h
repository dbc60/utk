/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#ifndef INCLUDED_DRIVER_TEST_H
#define INCLUDED_DRIVER_TEST_H

#include <but.h>
#include <ute_version.h>
//#include <ute_driver.h>

 // Function prototype typedefs
typedef const ch8 * (*get_version_str)();
typedef s32 (*get_version_num)();
//typedef b32 (*is_valid)(ute_context*);
//typedef ute_context * (*ute_context_new)(ute_test_suite *);
//typedef void (*ute_context_delete)(ute_context*);
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
//    ute_context            *utdd_ctx;
//    ute_test_suite         *utdd_ts;
//    ute_context_new         utdd_new;
//    ute_context_delete      utdd_delete;
    func_get_version_str   *utdd_get_version_str;
    func_get_version_num   *utdd_get_version_num;
//    is_valid                utdd_is_valid;
//    next                    utdd_next;
//    more                    utdd_more;
//    get_name_case           utdd_get_name_case;
//    get_name_suite          utdd_get_name_suite;
//    get_index               utdd_get_index;
//    get_count               utdd_get_count;
//    run_current             utdd_run_current;
//    get_count_passed        utdd_get_count_passed;
//    get_count_failed        utdd_get_count_failed;
//    get_count_failed_setup  utdd_get_count_failed_setup;
//    get_count_results       utdd_get_count_results;
//    get_result              utdd_get_result;
};
typedef struct ute_test_driver_data ute_test_driver_data;

extern but_test_case test_case_valid_version;
//extern but_test_case test_case_driver_load;
//extern but_test_case test_case_new_delete;
//extern but_test_case test_case_valid_context;
//extern but_test_case test_case_next;
//extern but_test_case test_case_name_case;
//extern but_test_case test_case_name_suite;
//extern but_test_case test_case_index;
//extern but_test_case test_case_count;
//extern but_test_case test_case_run;
//extern but_test_case test_case_results;

#endif  // INCLUDED_DRIVER_TEST_H
