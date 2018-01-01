/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief Methods for retrieving the version of this UTE library
 */

#ifndef UTE_VERSION_H_INCLUDED
#define UTE_VERSION_H_INCLUDED

#include "platform.h"

#define UTE_VERSION_MAJOR 0
#define UTE_VERSION_MINOR 1
#define UTE_VERSION_PATCH 0
#define UTE_VERSION_NUM ((UTE_VERSION_MAJOR << 16) | \
                         (UTE_VERSION_MINOR << 8) | \
                         UTE_VERSION_PATCH)
#define UTE_VERSION_STR STRINGIFY(UTE_VERSION_MAJOR) "." \
    STRINGIFY(UTE_VERSION_MINOR) "." STRINGIFY(UTE_VERSION_PATCH)

/**
 * @see UTE_VERSION_STR
 * @return the version of the UTE library as a string
 */
/* Returns UTE_VERSION_STR */
DllExport const ch8* ute_get_version_str(void);

/**
 * The UTE version encoded into an int, with the low order 8 bits being the
 * patch version, the next higher 8 bits being the minor version, and the next
 * higher 8 bits being the major version.
 * For example, 9.30.91 would be 0x00091E5B.
 *
 * @see UTE_VERSION_NUM
 * @return the version of the UTE library as a 32-bit unsigned int.
 */
/* Returns UTE_VERSION_NUM */
DllExport u32 ute_get_version_num(void);

#endif  /* UTE_VERSION_H_INCLUDED */
