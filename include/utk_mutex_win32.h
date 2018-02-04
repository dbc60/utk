/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief UTK mutex for Windows
 */

#ifndef WIN32_UTK_MUTEX_H_INCLUDED
#define WIN32_UTK_MUTEX_H_INCLUDED

#include "platform.h"
#include <Windows.h>


typedef volatile LONG utk_mutex;

enum {
    UTK_MUTEX_INITIALIZER = 0,
    UTK_MUTEX_UNLOCKED = UTK_MUTEX_INITIALIZER,
    UTK_MUTEX_LOCKED
};

/**
 * If the API was defined as functions:
 * 
 *  EXTERN_C void utk_mutex_lock(utk_mutex* handle);
 *  EXTERN_C int  utk_mutex_trylock(utk_mutex* handle);
 *  EXTERN_C void utk_mutex_unlock(utk_mutex* handle);
 */

/**
 *  Initialize a mutex like this:
 * 
 *      utk_mutex mutex = UTK_MUTEX_INITIALIZER;
 */

#define utk_mutex_lock(mutex)  do {                                     \
        while (UTK_MUTEX_UNLOCKED                                       \
                != InterlockedCompareExchange((mutex),                  \
                                              UTK_MUTEX_LOCKED,         \
                                              UTK_MUTEX_UNLOCKED)) {    \
            Sleep(0);                                                   \
        }                                                               \
    } while (0)
#define utk_mutex_trylock(mutex)    \
    ((int)InterlockedExchange((mutex), UTK_MUTEX_LOCKED))
#define utk_mutex_unlock(mutex)     \
    ((void)InterlockedExchange((mutex), UTK_MUTEX_UNLOCKED))

#endif  // WIN32_UTK_MUTEX_H_INCLUDED
