/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief A simple assertion mechanism based on excepions.
 */

#include "ehm.h"
#include "ehm_assert.h"

const DLL_EXPORT ehm_exception ehm_assert_failed = {"Assertion failed"};

void DLL_EXPORT ehm_assert(int e) {
    EHM_ASSERT(e);
}

