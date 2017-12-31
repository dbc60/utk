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
#include <dlfcn.h>

#define TEST_DRIVER_NAME                "Basic Unit Test Driver (Tux)"


int
main(int argc, char **argv)
{
    int result = 0;
    int i;
    ch8 *tspath;
    void *test_suite;
    but_test_suite_load *bts_load;
    but_test_suite *bts;
    const ch8 * version = but_get_version_str();

    printf("%s Version %s\n", TEST_DRIVER_NAME, version);

    if (argc > 1) {
        // Assume each argument is a path to a test suite
        for (i = 1; i < argc; ++i) {
            tspath = argv[i];
            test_suite = dlopen(tspath, RTLD_NOW | RTLD_LOCAL);

            if (test_suite) {
                bts_load = (but_test_suite_load*)
                           dlsym(test_suite, TEST_SUITE_LOAD_NAME);

                if (bts_load) {
                    bts = bts_load();
                    /*
                     * Most of the time we just want to see the error reports
                     * and move on to the next test suite. If we don't return
                     * zero from main, and we're launched from, say, nmake,
                     * then nmake will fail. It's better to let the calling
                     * program continue. rather than running something like
                     * result |= but_test_driver(bts);
                     */
                    (void)but_test_driver(bts);
                } else {
                    printf("Error: test suite %s doesn't export '%s'\n",
                           tspath,
                           TEST_SUITE_LOAD_NAME);
                }

                (void)dlclose(test_suite);
            } else {
                const ch8 *dlerr = dlerror();
                if (NULL == dlerr) {
                    dlerr = "unknown";
                }

                printf("Failed to load test suite %s, error = %s\n",
                       tspath, dlerr);
            }
        }
    } else {
        printf("Usage: %s <path to test suite>+\n", argv[0]);
    }

    return result;
}
