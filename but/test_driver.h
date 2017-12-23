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

extern but_test_case load_driver;
extern but_test_case test_validate_version;
extern but_test_case test_begin_end;
extern but_test_case test_is_valid;
extern but_test_case next_tc;
extern but_test_case case_name;
extern but_test_case suite_name;
extern but_test_case case_index;
extern but_test_case case_count;
extern but_test_case test_run;
extern but_test_case test_results;

#endif  // INCLUDED_DRIVER_TEST_H
