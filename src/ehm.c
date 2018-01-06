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
#include <stdlib.h>
#include <string.h>

// for snprintf, fprintf, stderr
#include <stdio.h>

//#include "lgm.h"
// Let's pretend we have a logging module. In the meantime, use fprintf here.
#define LGM_LOG_FATAL(s) fprintf(stderr, "%s\n", s)
#define LGM_LOG_ERROR(s) fprintf(stderr, "%s\n", s)

ehm_frame *ehm_stack = NULL;

#define LOG_BUF_SIZE_DEFAULT 128

size_t foo = sizeof(struct ehm_frame);

void ehm_throw(const ehm_exception *e,
               const char *filename,
               u32 line,
               const char *func)
{
    ehm_frame *frame = ehm_stack;

    EHM_ASSERT(e);
    if (NULL == frame)
    {
        /**
        * Ideally we'd have a logging module, and would build up strings.
        * Here we need to record/log/announce an uncaught exception
        */
        // We need a buffer of sufficient size
        int size_buf;
        char *log_buf = NULL;
        const char *format;
        const void *reason;
        int written = 0;
        char msg[] = "Unhandled exception";

        if (e->reason)
        {
            format = "%s %s thrown at %s:%d: %s, aborting.";
            reason = e->reason;
        }
        else
        {
            format = "%s at 0x%p thrown at %s:%d: %s, aborting.";
            reason = e;
        }

        size_buf = snprintf(log_buf, 0, format,
                            msg, reason, filename, line, func);
        // add space for the nul terminator
        ++size_buf;
        log_buf = malloc(size_buf);
        if (log_buf)
        {
            written = snprintf(log_buf, size_buf, format,
                               msg, reason, filename, line, func);
            if (written < 0 || written >= size_buf)
            {
                // The message was truncated
                LGM_LOG_ERROR("Error: fatal message truncated");
            }
        }
        else
        {
            log_buf = msg;
        }

        LGM_LOG_FATAL(log_buf);
        if (log_buf != msg)
        {
            free(log_buf);
        }
    }

    frame->exception = e;
    frame->file = filename;
    frame->line = line;
    frame->function = func;

    // pop the stack
    ehm_stack = ehm_stack->prev;

    // Throw the exception
    longjmp(frame->env, EHM_THROWN);
}
