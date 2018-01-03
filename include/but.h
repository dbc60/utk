/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief TBD
 */

#ifndef BUT_H_INCLUDED
#define BUT_H_INCLUDED

#include "platform.h"
#include "utk.h"


BEGIN_EXTERN_C
// Basic Unit Test (BUT)

/** @brief the test context */
typedef struct but_context but_context;

enum but_test_results {
    BTR_PASSED,         // The test case was run and it returned successfully
    BTR_FAILED,         // The test case was run and it returned a failure
    BTR_FAILED_EXC,     // The main test passed, but an exception path failed
    BTR_FAILED_SETUP,   // The setup function reported a failure
    BTR_NOT_RUN,        // The test case has not run
    BTR_CONTEXT_INVALID // Something is wrong with the test context
};
typedef enum but_test_results but_test_result;


END_EXTERN_C

#endif  /* BUT_H_INCLUDED */
