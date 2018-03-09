/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include <ute_counter.h>
#include <ute_driver.h>
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


EXTERN_C UTK_MALLOC_NO_ALIAS_ATTR UTK_MALLOC_CPTR_ATTR
void * utk_malloc(size_t size);

struct utk_counter {
    ute_counter ctr;
    size_t count_allocations;
    size_t count_invalid_free;
};
typedef struct utk_counter utk_counter;


const ehm_exception exc_invalid_argument = {"Invalid Argument"};
const ehm_exception exc_out_of_memory = {"Allocation Failure"};


void utk_counter_init(utk_counter *uc, ute_context *ctx);
void utk_counter_init(utk_counter *uc, ute_context *ctx) {
    ute_counter_init(&uc->ctr, ctx);
    uc->count_allocations = 0;
    uc->count_invalid_free = 0;
}

/** @brief the exception counter so ute_malloc_exceptions knows when to throw a
 * test exception.
 */
GLOBAL_VARIABLE utk_counter mem_counter;

/** @todo When do test exceptions get thrown and caught? They are the mechanism
 * through which the test driver records the results of what happens on each
 * exception path. The allocators must throw a test exception, I think, so the
 * test driver can process each exception path. How does the data flow in the
 * face of a test exception such that the code under test can continue to run
 * after the test exception is thrown?
 * 
 * Look at TestContext::run() in utkBaseline\src\ute\src\uteTestContext.cpp. It
 * runs setup, test and teardown. It catches UteAllocException and re-throws
 * it, but only as a means to provide a place for a debugger to set a break
 * point. UteAllocExceptions should be caught by the test allocator, which will
 * either throw std::bad_alloc for C++ "new", or return NULL for "new nothrow"
 * and C allocators. The run() method also catches and records unhandled
 * exceptions. Ideally, the run() method never catches an exception. Test
 * exceptions should be handled in the test allocator, and the code under test
 * should handle all other exceptions that it throws.
 * 
 * Also see the definition of class CppExceptionPoint in
 * utkBaseline\src\ute\src\uteExceptionPoint.h. An instance of this class is
 * what the run() method uses to get a test result, a count of invalid free
 * calls, and the test result for each exception path.
 */


INTERNAL_FUNCTION
void
utk_record_allocation(utk_counter *ctr, void *mem) {
    /** @brief record the allocation.
     * @todo add a container to utk_counter to record the address that was
     * allocated. If any memory leaks occur, the exact addresses can then be
     * reported by the test driver.
     */
    UNREFERENCED(mem);
    ++ctr->count_allocations;
}


UTK_MALLOC_NO_ALIAS_ATTR UTK_MALLOC_CPTR_ATTR
void *
ute_malloc_exceptions(size_t bytes)
{
    void *result = malloc(bytes);
    if (NULL == result) {
        EHM_THROW(exc_out_of_memory);
    }

    return result;
}


UTK_MALLOC_NO_ALIAS_ATTR UTK_MALLOC_CPTR_ATTR
void *
utk_malloc(size_t bytes)
{
    void *result;

    if (ute_would_throw(&mem_counter.ctr)) {
        result = NULL;
    } else {
        EHM_TRY {
            result = ute_malloc_exceptions(bytes);
            // log the allocation
            utk_record_allocation(&mem_counter, result);
        } EHM_CATCH(exc_out_of_memory) {
            result = NULL;
        } EHM_ENDTRY;
    }

    return result;
}   // uteMalloc


/** @todo Track file, function, and line number of each allocation request.
 * Design Question: should these values be tracked in a separate container that
 * contains the address of the memory allocated along with the file name,
 * function name, and line number where malloc, calloc, or realloc was called,
 * or should that information be passed down to an internal structures, such as
 * a header or footer in the "chunk" struct?
 */

/**
 * @brief accept the file, function, and line info from a tracking-allocator.
 */
UTK_MALLOC_NO_ALIAS_ATTR UTK_MALLOC_CPTR_ATTR
void *
ute_malloc_exceptions_track(size_t bytes,
                            const ch8 *name_file,
                            const ch8 *name_function,
                            size_t line)
{
    UNREFERENCED(name_file);
    UNREFERENCED(name_function);
    UNREFERENCED(line);
    void *result = malloc(bytes);
    if (NULL == result) {
        EHM_THROW(exc_out_of_memory);
    } else {
        /// @todo record file name, function, and line number.
        // shouldn't there be a function like:
        //
        //  utk_record_allocation(&mem_counter,
        //                        result, name_file, name_function, line);
        //
        // or a struct that records name_file, name_function, line and a copy
        // of it gets passed to a function like 'utk_record_allocation'?
        // utk_counter *is* a ute_counter with fields added for counting
        // allocations that haven't been freed, and a count of invalid free
        // calls. It will have to be expanded to include the memory address,
        // file name, function name and line number for each allocation that
        // does *not* have a matching free. It should also have that data for
        // each invalid free. Anything else?
        //
        // So each allocation will insert a new record, and each *valid* free
        // will remove one. Each invalid free will have its own set of records.
        // That means both utk_malloc_track and utk_free_track will have to
        // accept address, name_file, name_function, and line as arguments. In
        // the case of utk_free_track that data will be recorded only if the
        // address is invalid. Otherwise, it's used to remove a record that was
        // created by utk_malloc_track.
    }

    return result;
}


/**
 * @brief track the file, function, and line where an allocation was requested.
 * This should be helpful in debugging, but requires a macro defined in a
 * header file to capture and pass those values from the call site.
 */
UTK_MALLOC_NO_ALIAS_ATTR UTK_MALLOC_CPTR_ATTR
void *
utk_malloc_track(size_t bytes,
                 const ch8 *name_file,
                 const ch8 *name_function,
                 size_t line)
{
    void *result;

    if (ute_would_throw(&mem_counter.ctr)) {
        result = NULL;
    } else {
        EHM_TRY {
            result = ute_malloc_exceptions_track(bytes,
                                                 name_file,
                                                 name_function,
                                                 line);
            // log the allocation
            utk_record_allocation(&mem_counter, result);
        } EHM_CATCH(exc_out_of_memory) {
            result = NULL;
        } EHM_ENDTRY;
    }

    return result;
}   // uteMalloc


INTERNAL_FUNCTION
void
utk_record_invalid_free(utk_counter *ctr, void *mem) {
    b32 enabled = FALSE;

    ++ctr->count_invalid_free;
    if (ute_throw_is_enabled(&ctr->ctr)) {
        // Ensure we don't throw test exceptions when we're trying to record
        // an invalid call to free
        enabled = TRUE;
        ute_throw_disable(&ctr->ctr);
    }

    /** @brief record invalid free
     * @todo add a container to utk_counter to record invalid free addresses
     * that can later be reported by the test driver.
     */
    UNREFERENCED(mem);

    if (enabled) {
        // Re-enable throwing on exception points
        ute_throw_enable(&ctr->ctr);
    }
}


UTK_MALLOC_NO_ALIAS_ATTR
void
ute_free_exceptions(void *mem) {
    free(mem);
}

UTK_MALLOC_NO_ALIAS_ATTR void
utk_free(void *mem) {
    EHM_TRY {
        ute_free_exceptions(mem);
        --mem_counter.count_allocations;
    } EHM_CATCH(exc_invalid_argument) {
        // Count an attempt to free memory that is not currently allocated.
        utk_record_invalid_free(&mem_counter, mem);
    } EHM_ENDTRY;
}


INTERNAL_FUNCTION
void
utk_record_invalid_free_track(utk_counter *ctr, void *mem,
                              const ch8 *func, size_t line) {
    b32 enabled = FALSE;

    UNREFERENCED(func);
    UNREFERENCED(line);

    ++ctr->count_invalid_free;
    if (ute_throw_is_enabled(&ctr->ctr)) {
        // Ensure we don't throw test exceptions when we're trying to record
        // an invalid call to free
        enabled = TRUE;
        ute_throw_disable(&ctr->ctr);
    }

    /** @brief record invalid free
     * @todo add a container to utk_counter to record invalid free addresses
     * that can later be reported by the test driver.
     */
    UNREFERENCED(mem);

    if (enabled) {
        // Re-enable throwing on exception points
        ute_throw_enable(&ctr->ctr);
    }
}


UTK_MALLOC_NO_ALIAS_ATTR
void
ute_free_exceptions_track(void *mem, const ch8 *func, size_t line) {
    UNREFERENCED(func);
    UNREFERENCED(line);
    free(mem);
}

UTK_MALLOC_NO_ALIAS_ATTR
void
utk_free_track(void *mem, const ch8 *func, size_t line) {
    EHM_TRY {
        ute_free_exceptions_track(mem, func, line);
        --mem_counter.count_allocations;
    } EHM_CATCH(exc_invalid_argument) {
        // Count an attempt to free memory that is not currently allocated.
        utk_record_invalid_free_track(&mem_counter, mem, func, line);
    } EHM_ENDTRY;
}


//GLOBAL_VARIABLE utk_counter * const test_data = &mem_counter;

// setup and teardown methods
INTERNAL_FUNCTION utk_result setup(void *data);
INTERNAL_FUNCTION void teardown(void *data);

// test suite name and a forward reference to it.
#define UTK_TS_NAME "UTK Malloc"
GLOBAL_VARIABLE utk_test_suite utk_ts;

// test cases names
#define TEST_NAME_ALLOCATE_ONCE  "Allocate Once"
#define TEST_NAME_FAIL_ONCE "Fail Once"

// test methods
INTERNAL_FUNCTION utk_result test_alloc_once(void *data);
INTERNAL_FUNCTION utk_result test_fail_once(void *data);


//
// Test Cases
//

GLOBAL_VARIABLE utk_test_case test_case_alloc_once = {
    TEST_NAME_ALLOCATE_ONCE,
    setup,
    test_alloc_once,
    teardown,
    &mem_counter
};

GLOBAL_VARIABLE utk_test_case test_case_fail_once = {
    TEST_NAME_FAIL_ONCE,
    setup,
    test_fail_once,
    teardown,
    &mem_counter
};


//
// Test Results
//

enum test_results {
    UTK_MALLOC_SUCCESS = UTK_SUCCESS,
    UTK_MALLOC_UNEXPECTED_ALLOCATION,
    UTK_MALLOC_ALLOCATION_COUNT_INVALID,
//    UTK_MALLOC_INVALID_EXCEPTION_POINT_INITIAL,
//    UTK_MALLOC_INVALID_COUNT_THROW_INITIAL,
//    UTK_MALLOC_INVALID_THROW_TEST_EXCEPTION_INITIAL,
//    UTK_MALLOC_INVALID_CONTEXT,
//    UTK_MALLOC_INVALID_COUNT_THROW,
//    UTK_MALLOC_FAILED_TO_THROW,
//    UTK_MALLOC_FAILED_TO_THROW2,
//    UTK_MALLOC_INVALID_THROW,
//    UTK_MALLOC_FAILED_THROW_ENABLE,
    UTK_MALLOC_FAILED
};


//
// Setup and Teardown Methods
//

utk_result
setup(void *data) {
    utk_result result = UTK_MALLOC_SUCCESS;
    utk_counter *ctr = (utk_counter*)data;
    ute_context *ctx = ute_context_new(&utk_ts);

    if (NULL == ctx) {
        result = UTK_FAIL_SETUP;
    } else {
        utk_counter_init(ctr, ctx);
    }

    return result;
}

void
teardown(void *data) {
    utk_counter *ctr = (utk_counter*)data;
    ute_context *ctx = ute_counter_get_context(&ctr->ctr);
    ute_context_delete(ctx);
}


//
// Test Methods
//

utk_result
test_alloc_once(void *data) {
    utk_result result = UTK_MALLOC_FAILED;
    utk_counter *ctr = (utk_counter*)data;
    b32 count_correct = FALSE;
    void *mem;

    mem = utk_malloc(100);
    if (mem != NULL) {
        if (1 == ctr->count_allocations) {
            count_correct = TRUE;
        } else {
            result = UTK_MALLOC_ALLOCATION_COUNT_INVALID;
        }

        utk_free(mem);
        if (count_correct && 0 == ctr->count_allocations) {
            result = UTK_SUCCESS;
        }
    }

    return result;
}

utk_result
test_fail_once(void *data) {
    utk_result result = UTK_MALLOC_FAILED;
    utk_counter *ctr = (utk_counter*)data;
    void *mem;

    ute_increment_count_fail(&ctr->ctr);
    mem = utk_malloc(100);
    if (mem == NULL) {
        if (0 == ctr->count_allocations) {
            result = UTK_SUCCESS;
        }
    } else {
        utk_free(mem);
        result = UTK_MALLOC_UNEXPECTED_ALLOCATION;
    }

    return result;
}


GLOBAL_VARIABLE utk_test_case *tca[] = {
    &test_case_alloc_once,
    &test_case_fail_once
};


GLOBAL_VARIABLE utk_test_suite utk_ts = {
    UTK_TS_NAME,
    ARRAY_COUNT(tca),
    tca
};


/// @brief exported load function
PROJECTAPI utk_test_suite *
test_suite_load(void) {
    return &utk_ts;
}
