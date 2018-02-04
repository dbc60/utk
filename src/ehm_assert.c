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

#define EHM_SHARED_EXPORT 1
#include "ehm.h"
#include "ehm_assert.h"

const PROJECTAPI ehm_exception ehm_assert_failed = {"Assertion failed"};

void PROJECTAPI ehm_assert(int e) {
    EHM_ASSERT(e);
}

