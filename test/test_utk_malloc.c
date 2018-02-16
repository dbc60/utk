/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include <ute_counter.h>
#include <stdlib.h>


/** @def RESTRICT
* @brief Defined to the restrict keyword or equivalent if available
*/
#ifndef RESTRICT
  #if __STDC_VERSION__ >= 199901L		/* C99 or better */
    #define RESTRICT restrict
  #else
    #if defined(_MSC_VER) && _MSC_VER>=1400
      #define RESTRICT __restrict
    #endif
  
    #ifdef __GNUC__
      #define RESTRICT __restrict
    #endif
  #endif  // __STDC_VERSION__
  
  #ifndef RESTRICT
    #define RESTRICT
  #endif
#endif  // RESTRICT


#if defined(_MSC_VER) && _MSC_VER>=1400
  #define UTK_MALLOC_CPTR_ATTR __declspec(restrict)
  #define UTK_MALLOC_NO_ALIAS_ATTR __declspec(noalias)
#endif

#ifdef __GNUC__
  #define UTK_MALLOC_CPTR_ATTR __attribute__ ((malloc))
#endif

/** @def UTK_MALLOC_CPTR_ATTR
 * @brief Defined to the specifier for a pointer which points to a memory
 * block. Like UTK_MALLOC_NO_ALIAS_ATTR, but sadly not identical.
 */
#ifndef UTK_MALLOC_CPTR_ATTR
  #define UTK_MALLOC_CPTR_ATTR
#endif

/** @def UTK_MALLOC_NO_ALIAS_ATTR
 * @brief Defined to the specifier for a pointer which does not alias any other
 * variable.
 */
#ifndef UTK_MALLOC_NO_ALIAS_ATTR
  #define UTK_MALLOC_NO_ALIAS_ATTR
#endif


/** @def USE_MAGIC_HEADERS
 * @brief Defines whether utk_malloc should use magic headers in foreign heap
 * block detection
 *
 * USE_MAGIC_HEADERS causes utk_malloc to allocate an extra three
 * sizeof(size_t) to each block. utk_pfree() and utk_prealloc() can then
 * automagically know when to free a system allocated block. Enabling this
 * typically adds 20-50% to application memory usage, and is mandatory if
 * UTK_USE_ALLOCATOR is not 1.
 */
#ifndef USE_MAGIC_HEADERS
#define USE_MAGIC_HEADERS 0
#endif


/** @def UTK_USE_ALLOCATOR
 * @brief Defines the underlying allocator to use
 *
 * UTK_USE_ALLOCATOR can be one of these settings (it defaults to 1):
 *   0: System allocator (utk_malloc now simply acts as a threadcache) which is
 *      very useful for testing with valgrind and Glowcode.
 *      WARNING: Intended for DEBUG USE ONLY. Not all functions work correctly.
 *   1: utk_malloc
 */
#ifndef UTK_USE_ALLOCATOR
#define UTK_USE_ALLOCATOR 1 /* utk_malloc */
#endif

#if !UTK_USE_ALLOCATOR && !USE_MAGIC_HEADERS
#error If you are using the system allocator then you MUST use magic headers
#endif

/** @def UTK_REPLACE_SYSTEM_ALLOCATOR
 * @brief Defines whether to replace the system allocator (malloc(), free(),
 * et al) with utk_malloc's implementation.
 *
 * UTK_REPLACE_SYSTEM_ALLOCATOR on POSIX causes utk_malloc's functions to be
 * called malloc, free, etc. instead of utk_malloc, utk_free, etc. You may or
 * may not want this. On Windows, when built as a DLL, it causes utk_malloc to
 * patch all loaded DLLs and binaries to replace usage of the system allocator.
 *
 * Always turns on UTK_ENABLE_TOLERANCE.
 */

/** @def UTK_ENABLE_TOLERANCE
 * @brief Defines whether utk_malloc should check for blocks from the system
 allocator.

 * UTK_ENABLE_TOLERANCE is automatically turned on if
 * UTK_REPLACE_SYSTEM_ALLOCATOR is set or the Windows DLL is being built. This
 * causes utk_malloc to detect when a system allocator block is passed to it
 * and to handle it appropriately. Note that without USE_MAGIC_HEADERS there
 * is a very tiny chance that utk_malloc will segfault on non-Windows builds
 * (it uses Win32 SEH to trap segfaults on Windows and there is no comparable
 * system on POSIX).
 */

#ifdef UTK_REPLACE_SYSTEM_ALLOCATOR
#if UTK_USE_ALLOCATOR==0
#error Cannot combine using the system allocator with replacing the system allocator
#endif

#ifndef UTK_ENABLE_TOLERANCE
#define UTK_ENABLE_TOLERANCE 1
#endif

#define utk_aligned_alloc           aligned_alloc
#define utk_calloc                  calloc
#define utk_free                    free
#define utk_malloc                  malloc
#define utk_memalign                memalign
#define utk_posix_memalign          posix_memalign
#define utk_realloc                 realloc
#define utk_realloc_in_place        realloc_in_place
#define utk_valloc                  valloc
#define utk_pvalloc                 pvalloc
#define utk_mallinfo                mallinfo
#define utk_mallopt                 mallopt
#define utk_malloc_trim             malloc_trim
#define utk_malloc_stats            malloc_stats
#define utk_malloc_usable_size      malloc_usable_size
#define utk_malloc_footprint        malloc_footprint
#define utk_malloc_max_footprint    malloc_max_footprint
#define utk_malloc_footprint_limit  malloc_footprint_limit
#define utk_malloc_set_footprint_limit  malloc_set_footprint_limit
#define utk_malloc_inspect_all      malloc_inspect_all
#define utk_independent_calloc      independent_calloc
#define utk_independent_comalloc    independent_comalloc
#define utk_bulk_free               bulk_free
#endif  // UTK_REPLACE_SYSTEM_ALLOCATOR


EXTERN_C UTK_MALLOC_NO_ALIAS_ATTR UTK_MALLOC_CPTR_ATTR void * utk_malloc(size_t size);


GLOBAL_VARIABLE ute_counter mem_counter = {0};

UTK_MALLOC_NO_ALIAS_ATTR UTK_MALLOC_CPTR_ATTR void *
utk_malloc(size_t bytes)
{
    void *result;

    if (ute_would_throw(&mem_counter)) {
        result = NULL;
    } else {
        // Standard malloc behavior (N.B.: replace with utk_malloc_handled)
        result = malloc(bytes);
        if (result)
        {
            // log the allocation
            utk_record_allocation(&mem_counter, result);
        }
    }

    return result;
}   // uteMalloc
