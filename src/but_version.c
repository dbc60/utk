/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include <but_version.h>

const ch8 *
but_version(void)
{
    return BUT_VERSION;
}

s32
but_version_num(void)
{
    return BUT_VERSION_NUM;
}
