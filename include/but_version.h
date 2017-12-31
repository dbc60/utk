/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief Methods for retrieving the version of this BUT library
 */

#ifndef BUT_VERSION_H_INCLUDED
#define BUT_VERSION_H_INCLUDED

#include "platform.h"

#define BUT_VERSION_MAJOR 1
#define BUT_VERSION_MINOR 0
#define BUT_VERSION_PATCH 1
#define BUT_VERSION_NUM ((BUT_VERSION_MAJOR << 16) | \
                         (BUT_VERSION_MINOR << 8) | \
                         BUT_VERSION_PATCH)
#define BUT_VERSION_STR STRINGIFY(BUT_VERSION_MAJOR) "." \
    STRINGIFY(BUT_VERSION_MINOR) "." STRINGIFY(BUT_VERSION_PATCH)

/**
 * @see BUT_VERSION_STR
 * @return the version of hte BUT library as a string
 */
typedef const ch8* (but_version_get)(void);
DllExport but_version_get but_get_version_str; /* Returns BUT_VERSION_STR */

/**
 * The BUT version encoded into an int, with the low order 8 bits being the
 * patch version, the next higher 8 bits being the minor version, and the next
 * higher 8 bits being the major version.
 * For example, 9.30.91 would be 0x00091E5B.
 *
 * @see BUT_VERSION_NUM
 * @return the version of hte BUT library as a 32-bit unsigned int.
 */
typedef u32 (but_version_num_get)(void);
DllExport but_version_num_get but_get_version_num;  /* Returns BUT_VERSION_NUM */

#endif  /* BUT_VERSION_H_INCLUDED */
