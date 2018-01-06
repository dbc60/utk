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

#ifndef EHM_ASSERT_H_INCLUDED
#define EHM_ASSERT_H_INCLUDED

#ifdef NDEBUG
#define EHM_ASSERT(e) ((void)0)
#else
#include "ehm.h"

extern void ehm_assert(int e);
#define EHM_ASSERT(e) ((void)((e) || (EHM_THROW(ehm_assert_failed),0)))
#endif

extern const ehm_exception ehm_assert_failed;

#endif  // EHM_ASSERT_H_INCLUDED
