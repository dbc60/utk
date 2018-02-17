/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief An excepion handling module - Linux Shared Library.
 */

#include <platform.h>
#include <ehm.h>

// needed to ensure the linker has a reference to ehm_throw and friends.
void dummy_throw()
{
    b32 test = FALSE;
    EHM_TRY {
        EHM_ASSERT(test);
    } EHM_CATCH_ALL {
        ;
    } EHM_ENDTRY;
}
