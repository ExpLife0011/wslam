#include "Context.hpp"

_Use_decl_annotations_
void
FLTAPI
WslFlt::FileContext::Cleanup(
    _In_ PFLT_CONTEXT Context,
    _In_ FLT_CONTEXT_TYPE ContextType
)
{
    UNREFERENCED_PARAMETER(Context);
    UNREFERENCED_PARAMETER(ContextType);
    return;
}


_Use_decl_annotations_
void
FLTAPI
WslFlt::StreamContext::Cleanup(
    _In_ PFLT_CONTEXT Context,
    _In_ FLT_CONTEXT_TYPE ContextType
)
{
    UNREFERENCED_PARAMETER(Context);
    UNREFERENCED_PARAMETER(ContextType);
    return;
}


_Use_decl_annotations_
void
FLTAPI
WslFlt::StreamHandleContext::Cleanup(
    _In_ PFLT_CONTEXT Context,
    _In_ FLT_CONTEXT_TYPE ContextType
)
{
    UNREFERENCED_PARAMETER(Context);
    UNREFERENCED_PARAMETER(ContextType);
    return;
}
