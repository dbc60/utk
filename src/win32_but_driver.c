#include "platform.h"
#include "but.h"
#include "but_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


static void
test_driver_display_test_case(test_context *ctx)
{
    const ch8      *name_test_case;
    size_t          idx;

    name_test_case = test_context_get_name_test_case(ctx);
    idx = test_context_get_index(ctx);
    printf("%6d. \"%s\" %s\n", idx+1, name_test_case, " test case running.");
}


static int
but_test_driver(but_test_suite *bts)
{
    test_context   *ctx;
    size_t          i;
    int             result = 0;
    but_test_result result_test;
    const ch8      *suite_name;
    const ch8      *case_name;

    if (bts) {
        printf("\nTesting %s Package. Running %d Test Cases.\n",
               bts->name,
               bts->count);

        ctx = test_context_new(bts);
        if (test_context_is_valid(ctx)) {
            while (test_context_more_test_cases(ctx)) {
                // Display the name of the test case
                test_driver_display_test_case(ctx);
                test_context_run(ctx);
                i = test_context_get_index(ctx);
                result_test = test_context_get_result(ctx, i);
                if (result_test != 0) {
                    case_name = test_context_get_name_test_case(ctx);
                    if (result_test == BTR_FAILED_SETUP) {
                        printf("Error: could not setup %s: error code 0x%08x\n",
                               case_name,
                               test_context_get_error_code(ctx, i));
                    } else if (result_test == BTR_FAILED) {
                        printf("%s failed: error code = 0x%08x\n",
                               case_name,
                               test_context_get_error_code(ctx, i));
                    }
                }

                // Get the next test case
                test_context_next(ctx);
            }

            suite_name = test_context_get_name_test_suite(ctx);
            printf("\n%s Package Results\n\tTest Count:\t\
%d\n\tTests Run:\t%d\n\tTests Passed:\
\t%d\n\tTests Failed:\t%d\n\t\
Setups Failed:\t%d\n",
                   suite_name,
                   test_context_get_count_test_cases(ctx),
                   test_context_get_count_run(ctx),
                   test_context_get_count_passed(ctx),
                   test_context_get_count_failed(ctx),
                   test_context_get_count_failed_setup(ctx));
        } else {
            printf("Invalid test context\n");
            result = BTR_CONTEXT_INVALID;
        }
    } else {
        printf("No Test Suite\n");
        result = BTR_CONTEXT_INVALID;
    }

    return result;
}


// The command-line test driver interface
int
main(int argc, char **argv)
{
    int                 result = 0;
    int                 i;
    char               *tspath;
    HMODULE             library;
    but_test_suite_get  bts_get;
    but_test_suite     *bts;

    if (argc > 1) {
        // Assume each argument is a path to a test suite
        for (i = 1; i < argc; ++i) {
            tspath = argv[i];
            library = LoadLibraryA(tspath);

            if (library) {
                bts_get = (but_test_suite_get)GetProcAddress(library, "test_suite_get");
                if (bts_get) {
                    bts = bts_get();
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
                    printf("Error: test suite %s doesn't export 'test_suite_get'\n",
                           tspath);
                }

                FreeLibrary(library);
            } else {
                printf("Failed to load test suite %s, error = %d\n",
                       tspath, GetLastError());
            }
        }
    } else {
        printf("Usage: %s <path to test suite>+\n", argv[0]);
    }

    return result;
}
