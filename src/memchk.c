/* ========================================================================
 * (C) Copyright 2018 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief A memory management module. This is the checked implementation.
 * See mem.c for the production implementation.
 */

#include <stdlib.h>
#include <string.h>
#include "ehm.h"
#include "mem.h"

// checking types

/** @brief alignment
 * Ensure that any type of data can be stored in the blocks returned by
 * mem_alloc. If an address passed to mem_free isn't so aligned, it can't
 * possibly be in 'htab' and is thus invalid.
 */
union align {
    int i;
    long l;
    long long ll;
    long *lp;
    void *p;
    void (*fp)(void);
    float f;
    double d;
    long double ld;
};


// checking macros

#define hash(p, t) (((size_t)(p) >> 3) & (ARRAY_COUNT(t) - 1))
#define NDESCRIPTORS 512
#define NALLOC ((4096 + sizeof(union align) - 1) / (sizeof(union align))) \
    * (sizeof (union align))

const ehm_exception exception_mem_failed = {"Allocation failed"};
const ehm_exception exception_invalid_free = {"Invalid free"};


/** @brief a hash table of block descriptors.
 * 
 * - free: a list of descriptors on the free list; NULL if allocated and
 *         non-null if it's free.
 * - link: a list of descriptors that hash to the same index.
 * - ptr: the address of a block of memory.
 * - size: the number of bytes in the memory block.
 * - file: the name of the file where the allocation took place.
 * - line: the number of the line in the file where the allocation took place.
 * - func: the name of the function that allocated the memory block.
 */
struct descriptor {
    struct descriptor *free;
    struct descriptor *link;
    const void *ptr;
    size_t size;
    const char *file;
    size_t line;
    const char *func;
};

/** @brief hold all allocated and free blocks.
 */
static struct descriptor *htab[2048];

/** @brief a dummy descriptor that forms a circlularly linked list of free
 * descriptor blocks. It is the last descriptor on its list, and its free
 * field points to the first descriptor.
 */
static struct descriptor freelist = {&freelist};

static struct descriptor *
find(const void *ptr) {
    struct descriptor *result = htab[hash(ptr, htab)];

    while (result && result->ptr != ptr) {
        result = result->link;
    }

    return result;
}


static struct descriptor *
dalloc(void *ptr, size_t size,
       const char *file, size_t line, const char *func) {
    static struct descriptor *avail;
    static i64 nleft;

    if (nleft <= 0) {
        avail = malloc(NDESCRIPTORS * sizeof(*avail));
        if (avail) {
            nleft = NDESCRIPTORS;
        }
    } else {
        ++avail;
    }

    if (nleft > 0) {
        avail->ptr = ptr;
        avail->size = size;
        avail->file = file;
        avail->line = line;
        avail->func = func;
        avail->free = avail->link = NULL;
        --nleft;
    }

    return avail;
}


void *
mem_alloc(size_t bytes, const char *file, size_t line, const char *func) {
    struct descriptor *bp;
    void *ptr;
    void *result = NULL;

    // round up bytes to an alignment boundary
    bytes = ((bytes + sizeof(union align) - 1) / (sizeof(union align)))
            * (sizeof(union align));

    for (bp = freelist.free; bp && (NULL == result); bp = bp->free) {
        if (bp->size > bytes) {
            // use the end of the block at bp->ptr
            bp->size -= bytes;
            ptr = (u08*)bp->ptr + bp->size;
            if ((bp = dalloc(ptr, bytes, file, line, func)) != NULL) {
                size_t h = hash(ptr, htab);
                bp->link = htab[h];
                htab[h] = bp;

                // We found a block of memory
                result = ptr;
            } else {
                ehm_throw(&exception_mem_failed, file, line, func);
            }
        }

        if (NULL == result && bp == &freelist) {
            struct descriptor *newptr = NULL;

            if ((result = malloc(bytes + NALLOC)) == NULL
                || (newptr = dalloc(result, bytes + NALLOC, file, line, func)) 
                   == NULL) {
                ehm_throw(&exception_mem_failed, file, line, func);
            }

            newptr->free = freelist.free;
            freelist.free = result;
        }
    }

    return result;
}


void *
mem_calloc(size_t count, size_t bytes,
           const char *file, size_t line, const char *func) {
    size_t total = count * bytes;
    void *result;

    result = mem_alloc(total, file, line, func);
    memset(result, 0, total);

    return result;
}

void *
mem_resize(void *ptr, size_t bytes,
           const char *file, size_t line, const char *func) {
    struct descriptor *bp = NULL;
    void *result;

    EHM_ASSERT(ptr);

    if (((umm)ptr) % (sizeof(union align)) != 0
        || (bp = find(ptr)) == NULL
        || bp->free) {
        ehm_throw(&exception_invalid_free, file, line, func);
    }

    result = mem_alloc(bytes, file, line, func);
    memcpy(result, ptr, (bytes < bp->size ? bytes : bp->size));
    mem_free(ptr, file, line, func);
    return result;
}

void
mem_free(void *ptr, const char *file, size_t line, const char *func) {
    if (ptr) {
        struct descriptor *bp = NULL;

        if (((umm)ptr) % (sizeof(union align)) != 0
            || (bp = find(ptr)) == NULL
            || bp->free) {
            ehm_throw(&exception_invalid_free, file, line, func);
        }

        bp->free = freelist.free;
        freelist.free = bp;
    }
}
