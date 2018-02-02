/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief mutex for Windows
 */

#ifndef UTK_MUTEX_H_INCLUDED
#define UTK_MUTEX_H_INCLUDED

#if defined(_WIN32) || defined(_WIN64)
#include "win32_utk_mutex.h"
#elif defined(__GNUC__)
#include "linux_utk_mutex.h"
#else
#error Unknown compiler and target platform
#endif

#endif  // UTK_MUTEX_H_INCLUDED
