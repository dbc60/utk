/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief UTK mutex for Linux
 */

#ifndef LINUX_UTK_MUTEX_H_INCLUDED
#define LINUX_UTK_MUTEX_H_INCLUDED

#include "platform.h"
#include <pthread.h>


typedef pthread_mutex_t utk_mutex;

#define UTK_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER

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

#define utk_mutex_lock(mutex) pthread_mutex_lock(mutex)
#define utk_mutex_trylock(mutex) pthread_mutex_trylock(mutex)
#define utk_mutex_unlock(mutex) pthread_mutex_unlock(mutex)

#endif  // LINUX_UTK_MUTEX_H_INCLUDED
