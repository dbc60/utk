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

extern but_test_case test_case_driver_load;
extern but_test_case test_case_version_valid;
extern but_test_case test_case_new_delete;
extern but_test_case test_case_is_valid_context;
extern but_test_case test_case_next;
extern but_test_case test_case_name_case;
extern but_test_case test_case_name_suite;
extern but_test_case test_case_index;
extern but_test_case test_case_count;
extern but_test_case test_case_run;
extern but_test_case test_case_results;

#endif  // INCLUDED_DRIVER_TEST_H
