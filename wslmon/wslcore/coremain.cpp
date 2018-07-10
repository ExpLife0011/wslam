
#define WIN32_NO_STATUS
#include <Windows.h>
#undef WIN32_NO_STATUS
#include "wslcore_trace.h"
#include "coremain.cpp.tmh"
#include "TestHeader.hpp"
#include "FilterPort.hpp"
//#include "DriverComm.hpp"

BOOLEAN
WINAPI
DllMain(
    _In_ HINSTANCE hDllHandle,
    _In_ DWORD     nReason,
    _In_ LPVOID    Reserved
)
{
    UNREFERENCED_PARAMETER(Reserved);
    BOOLEAN bSuccess = TRUE;

    switch (nReason)
    {
    case DLL_PROCESS_ATTACH:
        //  For optimization.
        WPP_INIT_TRACING(L"WSLCORE");
        DisableThreadLibraryCalls(hDllHandle);
        break;
    case DLL_PROCESS_DETACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        WPP_CLEANUP();
        break;
    }

    return bSuccess;
}