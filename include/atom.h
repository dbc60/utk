/* ========================================================================
 * (C) Copyright 2018 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief An atom module.
 * 
 * An atom is a unique sequence of bytes. Often atoms are strings, but they can
 * be any sequential data.
 */

#ifndef ATOM_H_INCLUDED
#define ATOM_H_INCLUDED

#include "platform.h"

typedef struct atoms *atoms;
typedef size_t (*atom_hash)(const void *data, size_t bytes);

EXTERN_C atoms atoms_default();
EXTERN_C atoms atoms_new(size_t buckets, atom_hash);
EXTERN_C void atoms_free(atoms *at);
EXTERN_C size_t atom_length(atoms at, const char *str);
EXTERN_C const char *atom_data(atoms at, const void *data, size_t bytes);
EXTERN_C const char *atom_string(atoms at, const char *str);
EXTERN_C const char *atom_i64(atoms at, i64 n);
EXTERN_C const char *atom_i32(atoms at, i32 n);
EXTERN_C const char *atom_u64(atoms at, u64 n);
EXTERN_C const char *atom_u32(atoms at, u32 n);

#endif  // ATOM_H_INCLUDED
