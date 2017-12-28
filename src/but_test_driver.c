/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include <but.h>
#include <but_driver.h>
#include <but_version.h>
#include "but_test_driver.h"
#include <stdio.h>
#include <stdlib.h>

#define TEST_CASE_RUNNING_MSG           "test case running"
#define TEST_DRIVER_INVALID_CONTEXT     "invalid test context"
#define TEST_DRIVER_EMPTY_TEST_SUITE    "no test suite found"


void
test_driver_display_test_case(but_context *ctx)
{
    const ch8      *name_test_case;
    size_t          idx;

    name_test_case = but_get_name_test_case(ctx);
    idx = but_get_index(ctx);
    printf("%6zd. \"%s\" %s.\n", idx+1, name_test_case, TEST_CASE_RUNNING_MSG);
}


int
but_test_driver(but_test_suite *bts)
{
    but_context    *ctx;
    size_t          i;
    int             result = 0;
    but_test_result result_test;
    const ch8      *suite_name;
    const ch8      *case_name;

    if (bts) {
        printf("\nLoaded %s test suite. Running %zd Test Cases.\n",
               bts->name,
               bts->count);

        ctx = but_new(bts);
        if (but_is_valid(ctx)) {
            while (but_more_test_cases(ctx)) {
                // Display the name of the test case
                test_driver_display_test_case(ctx);
                but_run(ctx);
                i = but_get_index(ctx);
                result_test = but_get_result(ctx, i);
                if (result_test != 0) {
                    case_name = but_get_name_test_case(ctx);
                    if (result_test == BTR_FAILED_SETUP) {
                        printf("Error: could not setup %s: error code 0x%08x\n",
                               case_name,
                               but_get_error_code(ctx, i));
                    } else if (result_test == BTR_FAILED) {
                        printf("%s failed: error code = 0x%08x\n",
                               case_name,
                               but_get_error_code(ctx, i));
                    }
                }

                // Get the next test case
                but_next(ctx);
            }

            suite_name = but_get_name_test_suite(ctx);
            printf("\n%s Test Suite Results\n\tTest Count:\t\
%zd\n\tTests Run:\t%zd\n\tTests Passed:\
\t%zd\n\tTests Failed:\t%zd\n\t\
Setups Failed:\t%zd\n",
                   suite_name,
                   but_get_count_test_cases(ctx),
                   but_get_count_run(ctx),
                   but_get_count_passed(ctx),
                   but_get_count_failed(ctx),
                   but_get_count_failed_setup(ctx));
        } else {
            printf("Error: %s.\n", TEST_DRIVER_INVALID_CONTEXT);
            result = BTR_CONTEXT_INVALID;
        }
    } else {
        printf("Error: %s.\n", TEST_DRIVER_EMPTY_TEST_SUITE);
        result = BTR_CONTEXT_INVALID;
    }

    return result;
}
