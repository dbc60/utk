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

#ifndef EHM_H_INCLUDED
#define EHM_H_INCLUDED

#include "platform.h"
#include <setjmp.h>

// export/import symbol management
#if defined(PROJECT_WIN32)
#if defined(EHM_SHARED_EXPORT)
#define EHM_API __declspec(dllexport)
#else
#define EHM_API __declspec(dllimport)
#endif
#else
#define EHM_API
#endif


struct ehm_exception
{
    const ch8 *reason;
};
typedef struct ehm_exception ehm_exception;

struct ehm_frame
{
    jmp_buf env;
    struct ehm_frame *prev;
    const ch8 *file;
    const ch8 *function;
    const ehm_exception *exception;
    u32 line;
};
typedef struct ehm_frame ehm_frame;

#define EHM_FRAME_INIT(frame, stack, flag)  \
    (frame).prev = (stack);                 \
    (frame).file = NULL;                    \
    (frame).function = NULL;                \
    (frame).exception = NULL;               \
    (frame).line = 0;                       \
    (stack) = &(frame);                     \
    (flag) = setjmp((frame).env)

/** @brief there are four states within a TRY block */
enum ehm_states
{
    EHM_ENTERED,
    EHM_THROWN,
    EHM_HANDLED,
    EHM_FINALIZED
};
typedef enum ehm_states ehm_state;

/**
* The EHM_RETURN macro is used instead of return statements inside TRY blocks.
* It is an unchecked runtime error to execute a C return statement inside a
* TRY, CATCH, or FINALLY block. If any of the statements in must return, then
* they must do so via this macro.
*
* The switch statement is used in this macro so both 'EHM_RETURN;' and
* 'EHM_RETURN result;' expand to a valid C statement.
*/
#define EHM_RETURN                          \
    switch ((*ehm_stack) = (*ehm_stack)->prev, 0) \
    default:                                \
        return


#define EHM_TRY                         \
    do                                  \
    {                                   \
        volatile ehm_state ehm_flag;    \
        ehm_frame **ehm_stack;          \
        ehm_frame ehm_exc;              \
        ehm_stack = ehm_get_stack();    \
        EHM_FRAME_INIT(ehm_exc, (*ehm_stack), ehm_flag);   \
        if (EHM_ENTERED == ehm_flag)    \
        {

#define EHM_CATCH(e)                    \
        if (EHM_ENTERED == ehm_flag)    \
        {                               \
            (*ehm_stack) = (*ehm_stack)->prev;    \
        }                               \
    }                                   \
    else if (&(e) == ehm_exc.exception) \
    {                                   \
        ehm_flag = EHM_HANDLED;

#define EHM_CATCH_ALL                       \
    if (EHM_ENTERED == ehm_flag)            \
    {                                       \
        (*ehm_stack) = (*ehm_stack)->prev;  \
    }                                       \
    }                                       \
    else                                    \
    {                                       \
        ehm_flag = EHM_HANDLED;

#define EHM_FINALLY                         \
    if (EHM_ENTERED == ehm_flag)            \
    {                                       \
        (*ehm_stack) = (*ehm_stack)->prev;  \
    }                                       \
    }                                       \
    {                                       \
        if (EHM_ENTERED == ehm_flag)        \
        {                                   \
            ehm_flag = EHM_FINALIZED;       \
        }

#define EHM_ENDTRY                          \
    if (EHM_ENTERED == ehm_flag)            \
    {                                       \
        (*ehm_stack) = (*ehm_stack)->prev;  \
    }                                       \
    }                                       \
    if (EHM_THROWN == ehm_flag)             \
    {                                       \
        EHM_RETHROW;                        \
    }                                       \
    }                                       \
    while (0)

#define EHM_WHAT ((ehm_exc.exception->reason)     \
                      ? ehm_exc.exception->reason \
                      : "Unknown")
#define EHM_FILE (ehm_exc.file)
#define EHM_LINE (ehm_exc.line)
#define EHM_FUNC (ehm_exc.function)

EXTERN_C EHM_API void
ehm_throw(const ehm_exception *e, const ch8 *file, u32 line, const ch8 *func);

EXTERN_C EHM_API ehm_frame** ehm_get_stack(void);

/** @brief throw an exception.
* capture file name, line number, and function name, and pass them to the
* throw method.
*/
#define EHM_THROW(e) ehm_throw(&(e), __FILE__, __LINE__, __func__)

#define EHM_RETHROW              \
    ehm_throw(ehm_exc.exception, \
              ehm_exc.file,      \
              ehm_exc.line,      \
              ehm_exc.function)

#endif // EHM_H_INCLUDED
