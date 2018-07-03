#include <fltKernel.h>
#include "FilterUtils.hpp"
#include "wslflt_trace.h"
#include "FilterUtils.cpp.tmh"

_Use_decl_annotations_
WSLSTATUS
WslFlt::FilterUtils::OpenProcessByPid(
    _In_ _Const_ HANDLE ProcessId,
    _Out_ HANDLE *    ProcessHandle
)
{
    OBJECT_ATTRIBUTES objectAttribute = {};
    CLIENT_ID clientId = {};
    WSLSTATUS status = STATUS_UNSUCCESSFUL;

    InitializeObjectAttributes(
        &objectAttribute,
        nullptr,
        OBJ_KERNEL_HANDLE,
        nullptr,
        nullptr
    );

    clientId.UniqueProcess = ProcessId;
    clientId.UniqueThread = 0;

    WslLogTrace("ZwOpenProcess ProcessId=0x%p DesiredAccess=0x%X", ProcessId, PROCESS_ALL_ACCESS);
    status = ::ZwOpenProcess(
        ProcessHandle,
        PROCESS_ALL_ACCESS,
        &objectAttribute,
        &clientId
    );
    if (!WSL_SUCCESS(status))
    {
        WslLogError("ZwOpenProcess ProcessId=0x%p DesiredAccess=0x%x failed with status: 0x%x", ProcessId, PROCESS_ALL_ACCESS, status);
        *ProcessHandle = nullptr;
    }

    return status;
}


_Use_decl_annotations_
bool
WslFlt::FilterUtils::IsLinuxSubsystemProcess(
    _In_ _Const_ HANDLE ProcessId
)
{
    WSLSTATUS status = STATUS_UNSUCCESSFUL;
    ULONG processInformation = 0;
    HANDLE processHandle = nullptr;
    bool isLinuxSubsystemProcess = false;

    WslLogTrace("Wsl::FilterUtils::OpenProcessByPid ProcessId=0x%p", ProcessId);
    status = FilterUtils::OpenProcessByPid(ProcessId, &processHandle);
    if (!WSL_SUCCESS(status))
    {
        WslLogError(L"FilterUtils::OpenProcessByPid(ProcessId=0x%p) failed with status: 0x%X", ProcessId, status);
        goto CleanUp;
    }

    status = DynamicFunctions::ZwQueryProcessInformation(
        processHandle,
        ProcessSubsystemInformation,
        &processInformation,
        sizeof(processInformation),
        nullptr
    );
    if (!WSL_SUCCESS(status))
    {
        WslLogError("ZwQueryProcessInformation ProcessHandle(0x%p) failed with status: 0x%X", processHandle, status);
        goto CleanUp;
    }

    if (SubsystemInformationTypeWSL != processInformation)
    {
        goto CleanUp;
    }

    isLinuxSubsystemProcess = true;

CleanUp:
    if (processHandle)
    {
        ::ZwClose(processHandle);
    }

    return isLinuxSubsystemProcess;
}

_Use_decl_annotations_
WSLSTATUS
WslFlt::FilterUtils::GetProcessToken(
    _In_ PEPROCESS Process,
    _In_ ACCESS_MASK DesiredAccess,
    _Outptr_result_nullonfailure_ HANDLE *Token
)
{
    HANDLE processHandle = nullptr;

    *Token = nullptr;

    auto status = ::ObOpenObjectByPointer(Process, OBJ_KERNEL_HANDLE, nullptr, 0, nullptr, KernelMode, &processHandle);
    if (!WSL_SUCCESS(status))
    {
        return status;
    }

    status = GetProcessToken(processHandle, DesiredAccess, Token);
    ::ZwClose(processHandle);
    return status;
}

_Use_decl_annotations_
WSLSTATUS
WslFlt::FilterUtils::GetProcessToken(
    _In_ HANDLE Process,
    _In_ ACCESS_MASK DesiredAccess,
    _Outptr_result_nullonfailure_ HANDLE *Token
)
{
    HANDLE tokenHandle = nullptr;
    
    auto status = ::ZwOpenProcessTokenEx(Process, DesiredAccess, OBJ_KERNEL_HANDLE, &tokenHandle);
    *Token = tokenHandle;
    return status;
}

_Use_decl_annotations_
WSLSTATUS
WslFlt::FilterUtils::IsTokenElevated(
    _In_ HANDLE Token,
    _Out_ bool *Elevation
)
{
    TOKEN_ELEVATION elevation = { 0 };
    ULONG returnLength = 0;

    *Elevation = false;

    auto status = ::ZwQueryInformationToken(Token, TokenElevation, &elevation, sizeof(elevation), &returnLength);
    *Elevation = elevation.TokenIsElevated;
    return status;
}

_Use_decl_annotations_
WSLSTATUS
WslFlt::FilterUtils::TerminateProcess(
    _In_ HANDLE ProcessId
)
{
    HANDLE processHandle = nullptr;
    
    auto status = OpenProcessByPid(ProcessId, &processHandle);
    if (!WSL_SUCCESS(status))
    {
        return status;
    }

    status = ::ZwTerminateProcess(processHandle, STATUS_ACCESS_DENIED);
    ZwClose(processHandle);
    return status;
}

//_Use_decl_annotations_
//WSLSTATUS
//WslFlt::FilterUtils::GetProcessElevation(
//    _In_ PEPROCESS Process,
//    _Out_ bool* IsElevated
//)
//{
//    HANDLE tokenHandle = nullptr;
//
//    auto status = GetProcessToken(Process, TOKEN_READ, &tokenHandle);
//    if (!WSL_SUCCESS(status))
//    {
//        return status;
//    }
//
//    status = IsTokenElevated(tokenHandle, IsElevated);
//    ::ZwClose()
//    return status;
//}


