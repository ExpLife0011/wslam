#include "ProcessFilter.hpp"
#include "DynamicFunctions.hpp"
#include "FilterUtils.hpp"
#include "wslflt_trace.h"
#include "ProcessFilter.cpp.tmh"

using namespace WslFlt;

#pragma region FuncDefs

EXTERN_C
static
void PfpProcessCreatedCallback(
    _In_ HANDLE ParentId,
    _In_ HANDLE ProcessId,
    _Inout_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo
);

static
void
PfpHandleProcessCreate(
    _In_ HANDLE ParentId,
    _In_ HANDLE ProcessId,
    _Inout_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo
);

static
void
PfpHandleProcessTerminate(
    _In_ HANDLE ParentId,
    _In_ HANDLE ProcessId
);

EXTERN_C
_Function_class_(PCREATE_THREAD_NOTIFY_ROUTINE)
static
void PfpThreadCreatedCallback(
    _In_ HANDLE ProcessId,
    _In_ HANDLE ThreadId,
    _In_ BOOLEAN Create
);

static
void
PfpHandleThreadCreate(
    _In_ HANDLE ProcssId,
    _In_ HANDLE ThreadId
);

static
void
PfpHandleThreadTerminate(
    _In_ HANDLE ProcssId,
    _In_ HANDLE ThreadId
);

#pragma endregion


_Use_decl_annotations_
WSLSTATUS
WslFlt::ProcessFilter::StartProcessFiltering()
{
    WslLogInfo("Starting process filtering.");
    return ::PsSetCreateProcessNotifyRoutineEx2(
        PsCreateProcessNotifySubsystems,
        PfpProcessCreatedCallback,
        false
    );
}


_Use_decl_annotations_
WSLSTATUS
WslFlt::ProcessFilter::StopProcessFiltering()
{
    WslLogInfo("Stopping process filtering.");
    return ::PsSetCreateProcessNotifyRoutineEx2(
        PsCreateProcessNotifySubsystems,
        PfpProcessCreatedCallback,
        true
    );
}


_Use_decl_annotations_
WSLSTATUS
WslFlt::ProcessFilter::StartThreadFiltering()
{
    WslLogInfo("Starting thread filtering.");
    return ::PsSetCreateThreadNotifyRoutineEx(
        PsCreateThreadNotifySubsystems,
        PfpThreadCreatedCallback
    );
}


_Use_decl_annotations_
WSLSTATUS
WslFlt::ProcessFilter::StopThreadFiltering()
{
    WslLogInfo("Stopping thread filtering.");
    return ::PsRemoveCreateThreadNotifyRoutine(
        PfpThreadCreatedCallback
    );
}


EXTERN_C
static
void
PfpProcessCreatedCallback(
    _In_ HANDLE ParentId,
    _In_ HANDLE ProcessId,
    _Inout_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo
)
{
    
    if (!FilterUtils::IsLinuxSubsystemProcess(ProcessId))
    {
        return;
    }
    WslLogInfo("Filtered PICO PROCESS with PID 0x%p, PPID 0x%p", ProcessId, ParentId);

    if (CreateInfo)
    {
        PfpHandleProcessCreate(ParentId, ProcessId, CreateInfo);
    } else
    {
        PfpHandleProcessTerminate(ParentId, ProcessId);
    }
}


_Use_decl_annotations_
static
void 
PfpHandleProcessCreate(
    _In_ HANDLE ParentId,
    _In_ HANDLE ProcessId,
    _Inout_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo
)
{
    WSLSTATUS status = STATUS_UNSUCCESSFUL;
    PFLT_FILE_NAME_INFORMATION pFileNameInformation = nullptr;

    UNREFERENCED_PARAMETER(CreateInfo);
    UNREFERENCED_PARAMETER(ParentId);
    UNREFERENCED_PARAMETER(ProcessId);

    WslLogTrace("%wZ", &CreateInfo->FileObject->FileName);
    status = FltGetFileNameInformationUnsafe(
        CreateInfo->FileObject,
        nullptr,
        FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT,
        &pFileNameInformation
    );
    if (!WSL_SUCCESS(status))
    {
        return;
    }

    WslLogTrace("Normalized path %wZ", &pFileNameInformation->Name);

    FltReleaseFileNameInformation(pFileNameInformation);
    pFileNameInformation = nullptr;
}


_Use_decl_annotations_
static
void
PfpHandleProcessTerminate(
    _In_ HANDLE ParentId,
    _In_ HANDLE ProcessId
)
{
    UNREFERENCED_PARAMETER(ParentId);
    UNREFERENCED_PARAMETER(ProcessId);
}


EXTERN_C
_Use_decl_annotations_
static
void
PfpThreadCreatedCallback(
    _In_ HANDLE ProcessId,
    _In_ HANDLE ThreadId,
    _In_ BOOLEAN Create
)
{
    if (!FilterUtils::IsLinuxSubsystemProcess(ProcessId))
    {
        return;
    }

    WslLogInfo("Filtered PICO PROCESS THREAD with TID 0x%p, PID 0x%p", ThreadId, ProcessId);
    if (Create)
    {
        PfpHandleThreadCreate(ProcessId, ThreadId);
    } else
    {
        PfpHandleThreadTerminate(ProcessId, ThreadId);
    }
}


_Use_decl_annotations_
static
void
PfpHandleThreadCreate(
    _In_ HANDLE ProcessId,
    _In_ HANDLE ThreadId
)
{
    UNREFERENCED_PARAMETER(ProcessId);
    UNREFERENCED_PARAMETER(ThreadId);
}


_Use_decl_annotations_
static
void
PfpHandleThreadTerminate(
    _In_ HANDLE ProcessId,
    _In_ HANDLE ThreadId
)
{
    UNREFERENCED_PARAMETER(ProcessId);
    UNREFERENCED_PARAMETER(ThreadId);
}
