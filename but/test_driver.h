/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#ifndef INCLUDED_DRIVER_TEST_H
#define INCLUDED_DRIVER_TEST_H

#include <but_driver.h>
#include <but.h>

 // Function prototype typedefs
typedef const ch8 *(*get_version_string)(but_context*);
typedef const ch8 * (*get_version_str)();
typedef s32(*get_version_num)();
typedef int(*is_valid)(but_context*);
typedef but_context * (*but_context_new)(but_test_suite *);
typedef void(*but_context_delete)(but_context*);
typedef void(*next)(but_context*);
typedef b32(*more)(but_context*);
typedef const ch8 * (*get_name_case)(but_context*);
typedef const ch8 * (*get_name_suite)(but_context*);
typedef size_t(*get_index)(but_context* handle);
typedef size_t(*get_count)(but_context* handle);
typedef void(*run_current)(but_context* handle);
typedef size_t(*get_count_passed)(but_context*);
typedef size_t(*get_count_failed)(but_context*);
typedef size_t(*get_count_failed_setup)(but_context*);
typedef size_t(*get_count_results)(but_context*);
typedef but_test_result(*get_result)(but_context*, size_t);

// Test driver's data
typedef 
struct test_driver_data
{
    but_context            *tdd_ctx;
    but_test_suite         *tdd_ts;
    but_context_new         tdd_new;
    but_context_delete      tdd_delete;
    get_version_str         tdd_get_version_str;
    get_version_num         tdd_get_version_num;
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
} TestDriveData;

#define DRIVER_LIBRARY      "but_driver.dll"

/**
 * The names of the methods (BDM = BUT Driver Method) exported
 * from but_driver.dll
 */
#define BDM_GET_VERSION_STR         "but_version"
#define BDM_GET_VERSION_NUM         "but_version_num"
#define BDM_IS_VALID                "but_is_valid"
#define BDM_NEW                     "but_new"
#define BDM_DELETE                  "but_delete"
#define BDM_NEXT                    "but_next"
#define BDM_MORE_CASES              "but_more_test_cases"
#define BDM_GET_SUITE_NAME          "but_get_name_test_suite"
#define BDM_GET_CASE_NAME           "but_get_name_test_case"
#define BDM_GET_CASE_INDEX          "but_get_index"
#define BDM_GET_COUNT               "but_get_count_test_cases"
#define BDM_RUN                     "but_run"
#define BDM_GET_PASS_COUNT          "but_get_count_passed"
#define BDM_GET_FAIL_COUNT          "but_get_count_failed"
#define BDM_GET_SETUP_FAIL_COUNT    "but_get_count_failed_setup"
#define BDM_GET_RESULTS_COUNT       "but_get_count_results"
#define BDM_GET_RESULT              "but_get_result"

extern but_test_case test_case_driver_load;
extern but_test_case test_case_valid_version;
extern but_test_case test_case_new_delete;
extern but_test_case test_case_valid_context;
extern but_test_case test_case_next;
extern but_test_case test_case_name_case;
extern but_test_case test_case_name_suite;
extern but_test_case test_case_index;
extern but_test_case test_case_count;
extern but_test_case test_case_run;
extern but_test_case test_case_results;

#endif  // INCLUDED_DRIVER_TEST_H
