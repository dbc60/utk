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
typedef int64_t int64;

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
typedef uint64_t uint64;

typedef size_t memory_index;

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

/** @note: Originally, these three macros defined different usages of the
 * keyword static as 'internal', 'local_persist' and 'global_variable'. I've
 * changed them to all-caps and changed 'internal' to 'INTERNAL_FUNCTION,
 * because 'internal' is used as a field name in some Windows structures. For
 * example, see line 64 of the C++ header file "c:/Program Files
 * (x86)/Microsoft Visual Studio 14.0/VC/include/xiosbase".
 *
 * NOTE that the xiobase header file is included if your code includes the C++
 * <string> header. It may be included in other situations, too.
 */
#define INTERNAL_FUNCTION static
#define LOCAL_VARIABLE static
#define GLOBAL_VARIABLE static

// NOTE: '(Value - Value)' forces integral promotion to the size of Value
#define ALIGN_POW2(Value, Alignment)    \
    ((Value + ((Alignment) - 1)) & ~((Value - Value) + (Alignment) - 1))
#define ALIGN4(Value) ((Value + 3) & ~3)
#define ALIGN8(Value) ((Value + 7) & ~7)
#define ALIGN16(Value) ((Value + 15) & ~15)

/**
 * This is a convenience macro for generating opaque handles.
 *
 * This macro creates a forward declaration of a struct and defines a pointer
 * type to it. The pointer type is then usable as an opaque handle to specific
 * instances of the struct.
 */
#define DEFINE_HANDLE(name) typedef struct name name

#ifndef UNREFERENCED
#define UNREFERENCED(name) (name)
#endif

END_EXTERN_C

#endif  /* PLATFORM_H_INCLUDED */
