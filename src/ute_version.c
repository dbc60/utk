/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include <ute_version.h>

const ch8 *
ute_get_version_str(void)
{
    return UTE_VERSION_STR;
}

u32
ute_get_version_num(void)
{
    return UTE_VERSION_NUM;
}
