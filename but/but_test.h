/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#ifndef BUT_TEST_H_INCLUDED
#define BUT_TEST_H_INCLUDED

#include <but.h>

// Test the include guards
#ifndef BUT_H_INCLUDED
#error BUT_H_INCLUDED must be defined
#endif

extern but_test_case test_case_null;
extern but_test_case test_case_str;

#endif