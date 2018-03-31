/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief An exception handling module.
 */

#define EHM_SHARED_EXPORT 1
#include "ehm.h"
#include <platform.h>


LOCAL_VARIABLE ehm_frame *ehm_stack;

EHM_API ehm_frame**
ehm_get_stack()
{
    return &ehm_stack;
}

EHM_API void
ehm_throw(const ehm_exception *e,
          const ch8 *filename,
          umm line,
          const ch8 *func)
{
    ehm_frame *frame = ehm_stack;

    EHM_ASSERT(e);
    if (frame != NULL)
    {
        frame->exception = e;
        frame->file = filename;
        frame->line = line;
        frame->function = func;

        // pop the stack
        ehm_stack = ehm_stack->prev;

        // Throw the exception
        longjmp(frame->env, EHM_THROWN);
    }
}
