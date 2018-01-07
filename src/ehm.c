/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief An excepion handling module.
 */

#include "ehm.h"
#include "ehm_assert.h"
#include <platform.h>


DLL_EXPORT ehm_frame *ehm_stack;

DLL_EXPORT void
ehm_throw(const ehm_exception *e,
          const ch8 *filename,
          u32 line,
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
