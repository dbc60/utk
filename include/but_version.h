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

#define BUT_MAJOR_VERSION 0
#define BUT_MINOR_VERSION 0
#define BUT_PATCH_VERSION 1
#define BUT_VERSION_NUM ((BUT_MAJOR_VERSION << 16) | \
                         (BUT_MINOR_VERSION << 8) | \
                         BUT_PATCH_VERSION)
#define BUT_VERSION "0.0.1"

/**
 * @see BUT_VERSION
 * @return the version of hte BUT library as a string
 */
const ch8 * but_version(void); /* Returns BUT_VERSION */

/**
 * The BUT version encoded into an int, with the low order 8 bits being the
 * patch version, the next higher 8 bits being the minor version, and the next
 * higher 8 bits being the major version.
 * For example, 9.30.91 would be 0x00091E5B.
 *
 * @see BUT_VERSION_NUM
 * @return the version of hte BUT library as an int.
 */
s32 but_version_num(void);  /* Returns BUT_VERSION_NUM */

#endif  /* BUT_VERSION_H_INCLUDED */
