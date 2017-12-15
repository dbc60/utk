/* ========================================================================
   Author: Douglas B. Cuthbertson
   (C) Copyright 2015 by Douglas B. Cuthbertson. All Rights Reserved.
   ======================================================================== */


#include "platform.h"
#include "but.h"


// What kind of an internal struct do I need?
struct but_test_case
{
    wch                        *name;
    struct but_test_case_inf    interface;
    test_data                   data;
};

//! Move to but.c. This is the internal representation
struct but_test_suite
{
    wch                        *name;
    size_t                      count;
    struct but_test_case_inf    testCases;
};
