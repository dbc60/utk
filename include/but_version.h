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
#include "but.h"    // define DllExport

#define BUT_MAJOR_VERSION 0
#define BUT_MINOR_VERSION 0
#define BUT_PATCH_VERSION 1
#define BUT_VERSION_NUM ((BUT_MAJOR_VERSION << 16) | \
                         (BUT_MINOR_VERSION << 8) | \
                         BUT_PATCH_VERSION)
#define BUT_VERSION "0.1.0"

/**
 * @see BUT_VERSION
 * @return the version of hte BUT library as a string
 */
typedef const ch8* (but_version_get)(void);
DllExport but_version_get but_version; /* Returns BUT_VERSION */

/**
 * The BUT version encoded into an int, with the low order 8 bits being the
 * patch version, the next higher 8 bits being the minor version, and the next
 * higher 8 bits being the major version.
 * For example, 9.30.91 would be 0x00091E5B.
 *
 * @see BUT_VERSION_NUM
 * @return the version of hte BUT library as an int.
 */
typedef s32 (but_version_num_get)(void);
DllExport but_version_num_get but_version_num;  /* Returns BUT_VERSION_NUM */

#endif  /* BUT_VERSION_H_INCLUDED */
