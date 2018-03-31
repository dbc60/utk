/* ========================================================================
 * (C) Copyright 2018 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief A memory management module.
 */

#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

#include "platform.h"
#include "ehm.h"

EXTERN_C const ehm_exception exception_mem_failed;

#ifndef NDEBUG
EXTERN_C const ehm_exception exception_invalid_free;
#endif

EXTERN_C void *mem_alloc(size_t bytes,
                         const char *file, size_t line, const char *func);
EXTERN_C void *mem_calloc(size_t count, size_t bytes,
                          const char *file, size_t line, const char *func);
EXTERN_C void *mem_resize(void *ptr, size_t bytes,
                          const char *file, size_t line, const char *func);
EXTERN_C void mem_free(void *ptr,
                       const char *file, size_t line, const char *func);

#define ALLOC(nbytes)           \
    mem_alloc((nbytes), __FILE__, __LINE__, __func__)
#define CALLOC(count, nbytes)   \
    mem_calloc((count), (nbytes), __FILE__, __LINE__, __func__)
#define NEW(p)  ((p) = ALLOC(sizeof *(p)))
#define NEW0(p) ((p) = CALLOC(1, sizeof *(p)))

/// @brief release the memory and set the pointer to NULL
#define FREE(ptr)   \
    ((void)(mem_free((ptr), __FILE__, __LINE__, __func__), (ptr) = NULL))

#define RESIZE(ptr, nbytes) \
    ((ptr) = mem_resize((ptr), (nbytes), __FILE__, __LINE__, __func__))


#endif  // MEM_H_INCLUDED
