#include "FileFilter.hpp"
#include "Context.hpp"
#include "ContextBuilder.hpp"
#include "wslflt_trace.h"
#include "FileFilter.cpp.tmh"

volatile LONG WslFlt::FileFilter::_State = (LONG)WslFlt::FileFilter::FileFilterState::started;

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

_Use_decl_annotations_
FLT_POSTOP_CALLBACK_STATUS
WslFlt::FileFilter::WslPostCreate(
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_opt_ PVOID CompletionContext,
    _In_ FLT_POST_OPERATION_FLAGS Flags
)
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(Flags);

    NTSTATUS status = STATUS_UNSUCCESSFUL;
    LookasideObject<WslCompletionContext>* completionContext = (LookasideObject<WslCompletionContext>*)CompletionContext;
    FileContext *ctx = nullptr;

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
