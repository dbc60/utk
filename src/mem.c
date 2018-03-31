/* ========================================================================
 * (C) Copyright 2018 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief A memory management module. This is the production implementation.
 * See memchk.c for the checked implementation.
 */

#include <stdlib.h>
#include <stddef.h>
#include "ehm.h"
#include "mem.h"

const ehm_exception exception_mem_failed = {"Allocation failed"};

void *
mem_alloc(size_t bytes, const char *file, size_t line, const char *func) {
    void *result;

    result = malloc(bytes);
    if (result == NULL) {
        ehm_throw(&exception_mem_failed, file, line, func);
    }

    return result;
}

void *
mem_calloc(size_t count, size_t bytes,
           const char *file, size_t line, const char *func) {
    void *result;

    result = calloc(count, bytes);
    if (result == NULL) {
        ehm_throw(&exception_mem_failed, file, line, func);
    }

    return result;
}

void *
mem_resize(void *ptr, size_t bytes,
           const char *file, size_t line, const char *func) {
    void *result;

    EHM_ASSERT(ptr);

    result = realloc(ptr, bytes);

    if (result == NULL) {
        ehm_throw(&exception_mem_failed, file, line, func);
    }
    return result;
}

void
mem_free(void *ptr, const char *file, size_t line, const char *func) {
    UNREFERENCED(file);
    UNREFERENCED(line);
    UNREFERENCED(func);
    
    if (ptr) {
        free(ptr);
    }
}

