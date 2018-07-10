#include "FileFilter.hpp"
#include "Context.hpp"
#include "ContextBuilder.hpp"
#include "wslflt_trace.h"
#include "ProcessCollector.hpp"
#include "FilterUtils.hpp"
#include "FileFilter.cpp.tmh"
#include "WslFilter.hpp"


volatile LONG WslFlt::FileFilter::_State = (LONG)WslFlt::FileFilter::FileFilterState::started;
WslFlt::FltPort WslFlt::FileFilter::_Port{};

_Use_decl_annotations_
FLT_PREOP_CALLBACK_STATUS
WslFlt::FileFilter::WslPreCreate(
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID *CompletionContext
)
{
    LookasideObject<WslCompletionContext>* completionContext = nullptr;
    UNREFERENCED_PARAMETER(FltObjects);

    *CompletionContext = nullptr;

    if (Data->RequestorMode == KernelMode && FltGetRequestorProcessId(Data) == 4)
    {
        return FLT_PREOP_SUCCESS_NO_CALLBACK;
    }

    if (!FileFilter::IsMonitoring())
    {
        return FLT_PREOP_SUCCESS_NO_CALLBACK;
    }
    
    if (FlagOn(Data->Iopb->Parameters.Create.Options, FILE_DIRECTORY_FILE))
    {
        return FLT_PREOP_SUCCESS_NO_CALLBACK;
    }
    
    completionContext = FileFilter::CreateCompletionContext(Data);
    if (!completionContext)
    {
        return FLT_PREOP_SUCCESS_NO_CALLBACK;
    }
    *CompletionContext = completionContext;

    return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}

typedef struct _TERMINATE_PROCESS_CTX
{
    HANDLE ProcessId;
    KEVENT WaitEvent;
}TERMINATE_PROCESS_CTX, *PTERMINATE_PROCESS_CTX;

_Function_class_(KSTART_ROUTINE)
static
VOID
TerminateProcessWorker(
    _In_ PVOID StartContext
)
{
    PTERMINATE_PROCESS_CTX ctx = reinterpret_cast<PTERMINATE_PROCESS_CTX>(StartContext);
    OBJECT_ATTRIBUTES objectAttribute = { 0 };
    CLIENT_ID clientId = { 0 };
    HANDLE processHandle = nullptr;
    ULONG returnedLength = 0;
    PUNICODE_STRING pFileName = { 0 };
    UNICODE_STRING finalComponent = { 0 };

    InitializeObjectAttributes(
        &objectAttribute,
        nullptr,
        OBJ_KERNEL_HANDLE,
        nullptr,
        nullptr
    );

    clientId.UniqueProcess = ctx->ProcessId;
    clientId.UniqueThread = 0;

    ::ZwOpenProcess(
        &processHandle,
        PROCESS_ALL_ACCESS,
        &objectAttribute,
        &clientId
    );

    WslFlt::DynamicFunctions::ZwQueryProcessInformation(
        processHandle,
        ProcessImageFileName,
        nullptr,
        0,
        &returnedLength
    );

    pFileName = reinterpret_cast<PUNICODE_STRING>(::ExAllocatePoolWithTag(NonPagedPoolNx, returnedLength, 'NFI@'));

    WslFlt::DynamicFunctions::ZwQueryProcessInformation(
        processHandle,
        ProcessImageFileName,
        pFileName,
        returnedLength,
        &returnedLength
    );
    ::ZwClose(processHandle);
    

    ::FltParseFileName(pFileName, nullptr, nullptr, &finalComponent);

    WslFlt::FilterUtils::TerminateProcess(ctx->ProcessId);
    KeSetEvent(&ctx->WaitEvent, 0, false);
    WslFlt::FileFilter::_Port.SendDetection(&finalComponent);

    ::ExFreePoolWithTag(pFileName, 'NFI@');
}

_Use_decl_annotations_
FLT_POSTOP_CALLBACK_STATUS
WslFlt::FileFilter::WslPostCreate(
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_opt_ PVOID CompletionContext,
    _In_ FLT_POST_OPERATION_FLAGS Flags
)
{
    LookasideObject<WslCompletionContext>* completionContext = (LookasideObject<WslCompletionContext>*)CompletionContext;
    FileContext *ctx = nullptr;
    WslFlt::SharedPointer<WslFlt::Process> proc;
    auto status = WslFlt::ProcessCollector::Instance().GetProcessByPid((HANDLE)FltGetRequestorProcessId(Data), proc);
    if (!WSL_SUCCESS(status))
    {
        goto Cleanup;
    }

    HANDLE token = nullptr;
    auto eproc = FltGetRequestorProcess(Data);

    status = WslFlt::FilterUtils::GetProcessToken(eproc, TOKEN_READ, &token);
    if (!WSL_SUCCESS(status))
    {
        goto Cleanup;
    }

    auto isElevated = false;

    status = WslFlt::FilterUtils::IsTokenElevated(token, &isElevated);
    ::ZwClose(token);
    if (!WSL_SUCCESS(status))
    {
        goto Cleanup;
    }

    if (isElevated && !proc->IsElevated())
    {
        //__debugbreak();
        // runtime elevation occured
        HANDLE threadHandle = nullptr;
        OBJECT_ATTRIBUTES attributes = { 0 };

        TERMINATE_PROCESS_CTX terminationContext = { 0 };

        terminationContext.ProcessId = proc->ProcessId();
        ::KeInitializeEvent(&terminationContext.WaitEvent, NotificationEvent, false);

        InitializeObjectAttributes(&attributes, nullptr, OBJ_KERNEL_HANDLE, nullptr, nullptr);
        ::PsCreateSystemThread(&threadHandle, GENERIC_ALL, &attributes, nullptr, nullptr, TerminateProcessWorker, &terminationContext);

        ::KeWaitForSingleObject(&terminationContext.WaitEvent, Executive, KernelMode, false, nullptr);
        ::ZwWaitForSingleObject(threadHandle, false, nullptr);
        ::ZwClose(threadHandle);
    }

    //WslLogInfo("WslPostCreate Path = %wZ, Requestor = 0x%x", &FltObjects->FileObject->FileName, FltGetRequestorProcessId(Data));

    if (!FltSupportsFileContexts(Data->Iopb->TargetFileObject))
    {
        goto Cleanup;
    }


    status = ContextBuilder::GetOrSetFileContext(Data, NonPagedPoolNx, &ctx);
    if (!WSL_SUCCESS(status))
    {
        WslLogError("WslFlt::ContextBuilder::GetOrSetFileContext Data=0x%p, PoolType=NonPagedPoolNx, Context=0x%p failed with status: 0x%x", Data, ctx, status);
        goto Cleanup;
    }

Cleanup:
    if (ctx)
    {
        FltReleaseContext(ctx);
    }

    delete completionContext;

    return FLT_POSTOP_FINISHED_PROCESSING;
}

_Use_decl_annotations_
WslFlt::LookasideObject<WslFlt::WslCompletionContext>*
WslFlt::FileFilter::CreateCompletionContext(
    _In_ PFLT_CALLBACK_DATA Data
)
{
    LookasideObject<WslCompletionContext>* completionContext = new LookasideObject<WslCompletionContext>{};
    if (!completionContext)
    {
        return nullptr;
    }

    completionContext->Requestor = FltGetRequestorProcessId(Data);

    return completionContext;
}
