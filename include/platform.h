/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief a variety of convenient data types and macros
 */

#ifndef PLATFORM_H_INCLUDED
#define PLATFORM_H_INCLUDED

#if defined(__cplusplus)
// Single line extern "C"
#ifndef EXTERN_C
#define EXTERN_C    extern "C"
#endif

// Block of extern "C"
#define BEGIN_EXTERN_C extern "C" {
#define END_EXTERN_C }

#else   // __cplusplus

// Single line extern "C"
#ifndef EXTERN_C
#define EXTERN_C    extern
#endif

// Block of extern "C"
#define BEGIN_EXTERN_C
#define END_EXTERN_C
#endif  // __cplusplus


BEGIN_EXTERN_C
#include <limits.h>
#include <stdint.h>
#include <stddef.h>


typedef int8_t  int8;
typedef int8_t  int08;
typedef int16_t int16;
typedef int32_t int32;

/** A bit of gcc insanity
 * The size of int64_t and "long long int" are both 8 bytes. However, when it
 * comes to printf format strings, gcc insists int64_t variables use "%ld",
 * and "long long int" use "%lld". Under Visual Studio, both are formatted with
 * "%lld".
 */
#ifdef __GNUC__
typedef long long int int64;
#else
typedef int64_t int64;
#endif

/**
 * the intent of bool32 is to be zero for false and non-zero for true.
 */
typedef int32_t bool32;

#undef FALSE
#define FALSE ((bool32)0)

#undef TRUE
#define TRUE ((bool32) 1)

typedef uint8_t  uint8;
typedef uint8_t  uint08;
typedef uint16_t uint16;
typedef uint32_t uint32;
/** A bit of gcc insanity
 * The size of uint64_t and "long long unsigned int" are both 8 bytes. However,
 * when it comes to printf format strings, gcc insists uint64_t variables use
 * "%lu", and "long long unsigned int" use "%llu". Under Visual Studio, both
 * are formatted with "%lld".
*/
#ifdef __GNUC__
typedef long long unsigned int uint64;
#else
typedef uint64_t uint64;
#endif

typedef size_t memory_index;
typedef uint32 counter32;

typedef uintptr_t   umm;
typedef intptr_t    smm;

typedef float real32;
typedef double real64;

/** @note: Very brief type names. */
typedef int8    s8;
typedef int8    s08;
typedef int16   s16;
typedef int32   s32;
typedef int64   s64;

typedef int8    i8;
typedef int8    i08;
typedef int16   i16;
typedef int32   i32;
typedef int64   i64;

typedef uint8   u08;
typedef uint16  u16;
typedef uint32  u32;
typedef uint64  u64;

typedef bool32  b32;
typedef char    ch8;
typedef signed char sch;
typedef unsigned char uch;
typedef wchar_t wch;
typedef real32  r32;
typedef real64  r64;

// Complement the definition of CHAR_BIT
#define U08_BIT CHAR_BIT
#define U16_BIT (sizeof(u16) * U08_BIT)
#define U32_BIT (sizeof(u32) * U08_BIT)
#define U64_BIT (sizeof(u64) * U08_BIT)
#define S16_BIT (sizeof(s16) * U08_BIT)
#define S32_BIT (sizeof(s32) * U08_BIT)
#define S64_BIT (sizeof(s64) * U08_BIT)

#define U08_MASK ((u08)(~0))
#define U16_MASK ((u16)(~0))
#define U16_MASK_HIGH   0xFF00
#define U16_MASK_LOW    0x00FF
#define U32_MASK ((u32)(~0))
#define U32_MASK_HIGH   0xFFFF0000
#define U32_MASK_LOW    0x0000FFFF
#define U64_MASK ((u64)(~0))
#define U64_MASK_HIGH   0xFFFFFFFF00000000
#define U64_MASK_LOW    0x00000000FFFFFFFF
#define S08_MASK ((s08)(~0))
#define S16_MASK ((s16)(~0))
#define S32_MASK ((s32)(~0))
#define S64_MASK ((s64)(~0))

#define BYTES_PER_U16 (sizeof(u16) / sizeof(u08))

#define KILOBYTES(V) ((V) * 1024LL)
#define MEGABYTES(V) (KILOBYTES(V) * 1024LL)
#define GIGABYTES(V) (MEGABYTES(V) * 1024LL)
#define TERABYTES(V) (GIGABYTES(V) * 1024LL)

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

/** @brief A stringification macro for C or C++.
 * STRINGIFY converts its argument to a string.
 */
#define STRINGIFY_HELPER(X) #X
#define STRINGIFY(X) STRINGIFY_HELPER(X)

/** @brief: These macros define different usages of the keyword static */
#define INTERNAL_FUNCTION static
#define LOCAL_VARIABLE static
#define GLOBAL_VARIABLE static

/** @brief round up Value to the next multiple of AlignPow2
 * where AlignPow2 is a positive power of 2 (i.e., 2, 4, 8, 16, 32, etc.).
 * NOTE: '(Value - Value)' forces integral promotion to the size of Value.
 */
#define ALIGN_POW2(Value, AlignPow2)    \
    ((Value + ((AlignPow2) - 1)) & ~((Value - Value) + (AlignPow2) - 1))

/** @brief round Value to the next multiple of 4 */
#define ALIGN4(Value) ((Value + 3) & ~3)

/** @brief round Value to the next multiple of 8 */
#define ALIGN8(Value) ((Value + 7) & ~7)

/** @brief round Value to the next multiple of 16 */
#define ALIGN16(Value) ((Value + 15) & ~15)

/** @brief use to prevent warnings about unreferenced parameters.
 * Sometimes functions have parameters that aren't used, but have to exist so
 * their signatures match some prototype or typedef. Unused parameters can
 * cause spurious compiler warnings. This macro can be used to mark such
 * parameters and thus stifle these noisy compiler warnings.
 */
#ifndef UNREFERENCED
#define UNREFERENCED(arg) ((void)(arg))
#endif

/** @brief if PROJECT_WIN32 is defined, then define import/export macros */
#ifdef PROJECT_WIN32
#define DLL_IMPORT   __declspec(dllimport)
#define DLL_EXPORT  __declspec(dllexport)

#ifdef PROJECTLIBRARY_EXPORTS
#define PROJECTAPI __declspec(dllexport)
#else
#define PROJECTAPI __declspec(dllimport)
#endif

#else   // PROJECT_WIN32
#define DLL_IMPORT
#define DLL_EXPORT
#define PROJECTAPI
#endif  // PROJECT_WIN32


END_EXTERN_C

#endif  /* PLATFORM_H_INCLUDED */
