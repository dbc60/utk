/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief An excepion handling module - Windows Shared Library.
 */

#include <platform.h>
#define EHM_SHARED_EXPORT 1
#include <ehm.h>

#include <SDKDDKVer.h>

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
// Windows Header Files:
#include <windows.h>
#include <stdlib.h>


/** @brief force the linker to include 'ehm_throw' so it may be exported
 * by this DLL
 */
void dummy_throw()
{
    b32 test = FALSE;
    EHM_TRY{
        EHM_ASSERT(test);
    } EHM_CATCH_ALL{
        ;
    } EHM_ENDTRY;
}


BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
)
{
    UNREFERENCED(hModule);
    UNREFERENCED(lpReserved);

    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        dummy_throw();
        break;
    
    case DLL_THREAD_ATTACH:
        /** @todo Consider calling TlsAlloc to allocate a Thread Local Storage
         * index to hold a pointer to a per-thread ehm_frame. Set the per-
         * thread frame to NULL. The EHM_* macros will have to be updated to
         * manage per-thread ehm_frame objects.
         */
        break;

    case DLL_THREAD_DETACH:
        /** @todo Call TlsFree to release the TLS index allocated in
         * DLL_THREAD_ATTACH.
         */
        break;
    
    case DLL_PROCESS_DETACH:
        break;
    }
    
    return TRUE;
}
