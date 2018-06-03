#include "ContextBuilder.hpp"
#include "WslFilter.hpp"
#include "wslflt_trace.h"
#include "ContextBuilder.cpp.tmh"

_Use_decl_annotations_
WSLSTATUS
WslFlt::ContextBuilder::BuildFileContext(
    _In_ POOL_TYPE PoolType,
    _Out_ FileContext** Context
    )
{
    WSLSTATUS status = STATUS_UNSUCCESSFUL;

    status = ::FltAllocateContext(
        WslFilter::Instance().Filter(),
        FLT_FILE_CONTEXT,
        ContextTraits<FileContext>::Size,
        PoolType,
        (PFLT_CONTEXT *)Context
    );

    return status;
}

_Use_decl_annotations_
WSLSTATUS
WslFlt::ContextBuilder::GetOrSetFileContext(
    _In_ PFLT_CALLBACK_DATA Data,
    _In_ POOL_TYPE PoolType,
    _Out_ FileContext** Context
)
{
    WSLSTATUS status = STATUS_UNSUCCESSFUL;
    FileContext *ctx = nullptr;
    FileContext *oldCtx = nullptr;

    *Context = nullptr;

    status = ::FltGetFileContext(
        Data->Iopb->TargetInstance,
        Data->Iopb->TargetFileObject,
        (PFLT_CONTEXT*)&ctx
    );
    if (!WSL_SUCCESS(status) && STATUS_NOT_FOUND != status)
    {
        WslLogError("FltGetFileContext Instance=0x%p, FileObject=0x%p, Context=0x%p failed with status: 0x%x", Data->Iopb->TargetInstance, Data->Iopb->TargetFileObject, ctx, status);
        goto Cleanup;
    }

    if (STATUS_NOT_FOUND == status)
    {
        status = ContextBuilder::BuildFileContext(PoolType, &ctx);
        if (!WSL_SUCCESS(status))
        {
            WslLogError("FltAllocateContext ContextType=FLT_FILE_CONTEXT, Size=0x%x, PoolType=0x%x, Context=0x%p failed with status: 0x%x", ContextTraits<FileContext>::Size, PoolType, ctx, status);
            goto Cleanup;
        }

        status = ::FltSetFileContext(
            Data->Iopb->TargetInstance,
            Data->Iopb->TargetFileObject,
            FLT_SET_CONTEXT_KEEP_IF_EXISTS,
            ctx,
            (PFLT_CONTEXT*)&oldCtx
        );
        if (!WSL_SUCCESS(status) && STATUS_FLT_CONTEXT_ALREADY_DEFINED != status)
        {
            WslLogError("FltSetFileContext Instance=0x%p, FileObject=0x%p, Operation=FLT_SET_CONTEXT_KEEP_IF_EXISTS, Context=0x%p, OldContext=0x%p, failed with status: 0x%x", Data->Iopb->TargetInstance, Data->Iopb->TargetFileObject, ctx, oldCtx, status);
            goto Cleanup;
        }
        if (STATUS_FLT_CONTEXT_ALREADY_DEFINED == status)
        {
            ::FltReleaseContext(ctx);
            ctx = oldCtx;
            oldCtx = nullptr;
        }
    }

    status = STATUS_SUCCESS;
    *Context = ctx;

Cleanup:
    if (!WSL_SUCCESS(status))
    {
        if (ctx)
        {
            ::FltReleaseContext(ctx);
            ctx = nullptr;
        }

        if (oldCtx)
        {
            ::FltReleaseContext(oldCtx);
            oldCtx = nullptr;
        }
    }

    return status;
}
