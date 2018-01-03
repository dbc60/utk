/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include "ute_counter.h"
#include <string.h>

void
ute_counter_init(ute_counter * uc, ute_context * ctx)
{
    memset(uc, 0, sizeof *uc);
    uc->context = ctx;
}

ute_context *
ute_counter_get_context(ute_counter * uc) {
    return uc->context;
}