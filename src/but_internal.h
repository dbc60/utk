/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#pragma once

#include "platform.h"
#include "but.h"


// What kind of an internal struct do I need?
struct but_test_case
{
    wch                        *name;
    struct but_test_case_inf    interface;
    test_data                  *data;
};

//! Move to but.c. This is the internal representation
struct but_test_suite
{
    wch                        *name;
    size_t                      count;
    struct but_test_case_inf    testCases;
};
