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
    ::ZwClose(processHandle);

    return isLinuxSubsystemProcess;
}

