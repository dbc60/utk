/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief platform-independent data types and macros
 */

#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

/*
 * CEILING calculates the smallest multiple of m greater than or equal to v,
 * where m is a power of 2
 */
#define CEILING(v,m)   (((v)+(m)-1)&~((m)-1))

/*
 * FLOOR calculates the largest multiple of m less than or equal to v, where
 * m is a power of 2.
 */
#define FLOOR(v,m)  ((v)&~((m)-1))

#endif  // UTILITIES_H_INCLUDED
