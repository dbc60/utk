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
#include <ehm.h>
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

void
test_driver_display_unhandled_exception(but_context *ctx,
                                        const char *reason,
                                        const char *file,
                                        const char *function,
                                        u32 line)
{
    const ch8 *name_test_case;
    ch8 msg[] = "Unhandled exception";
    const ch8 format[] = "%s occurred in test case \"%s\". Exception \"%s\" thrown at %s:%d: %s.";
    int size_buf = 0;
    ch8 *log_buf = NULL;
    int written = 0;

    name_test_case = but_get_name_test_case(ctx);
    /*
    * size_buf is zero, so snprintf will return the number of bytes
    * that would have been written, excluding the nul terminator.
    */
    size_buf = snprintf(log_buf, size_buf, format,
                        msg, name_test_case, reason, file, line, function);
    // add space for the nul terminator
    ++size_buf;
    log_buf = malloc(size_buf);
    if (log_buf) {
        written = snprintf(log_buf, size_buf, format,
                           msg, name_test_case, reason, file, line, function);
        if (written < 0 || written >= size_buf) {
            // The message was truncated
            printf("Error: fatal message truncated\n");
        }
    } else {
        log_buf = msg;
    }

    printf("%s\n", log_buf);
    if (log_buf != msg) {
        free(log_buf);
    }
}

int
but_test_driver(utk_test_suite *bts)
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
                EHM_TRY {
                    but_run(ctx);
                    i = but_get_index(ctx);
                    result_test = but_get_result(ctx, i);
                } EHM_CATCH_ALL {
                    i = but_get_index(ctx);
                    result_test = BTR_UNHANDLED_EXCEPTION;
                } EHM_FINALLY {
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
                        } else if (result_test == BTR_UNHANDLED_EXCEPTION) {
                            test_driver_display_unhandled_exception(ctx,
                                                                    EHM_WHAT,
                                                                    EHM_FILE,
                                                                    EHM_FUNC,
                                                                    EHM_LINE);
                            insert_result(ctx,
                                          BTR_UNHANDLED_EXCEPTION,
                                          result_test);
                        }
                    }
                } EHM_ENDTRY;

                // Get the next test case
                but_next(ctx);
            }

            suite_name = but_get_name_test_suite(ctx);
            printf("\n%s Test Suite Results\n\tTest Count:\t\
%zd\n\tTests Run:\t%zd\n\tTests Passed:\
\t%zd\n\tTests Failed:\t%zd\n\t\
Setups Failed:\t%zd\n\
\tExceptions:\t%zd\n",
                   suite_name,
                   but_get_count_test_cases(ctx),
                   but_get_count_run(ctx),
                   but_get_count_passed(ctx),
                   but_get_count_failed(ctx),
                   but_get_count_failed_setup(ctx),
                   but_get_count_unhandled_exceptions(ctx));
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
