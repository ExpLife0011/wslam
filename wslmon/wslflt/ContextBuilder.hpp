#ifndef _CONTEXT_BUILDER_HPP_
#define _CONTEXT_BUILDER_HPP_

#include "Context.hpp"
#include "wsl_status.hpp"

namespace WslFlt
{
    class ContextBuilder
    {
    public:

        template<FLT_CONTEXT_TYPE _Type>
        _Must_inspect_result_
        static
        WSLSTATUS
        BuildContext(
            _In_ POOL_TYPE PoolType,
            _Out_ FileContext** Context
        );

        _Must_inspect_result_
        static
        WSLSTATUS
        BuildFileContext(
            _In_ POOL_TYPE PoolType,
            _Out_ FileContext** Context
        );

        _Must_inspect_result_
        static
        WSLSTATUS
        GetOrSetFileContext(
            _In_ PFLT_CALLBACK_DATA Data,
            _In_ POOL_TYPE PoolType,
            _Out_ FileContext** Context
        );
    };

    template<FLT_CONTEXT_TYPE _Type>
    inline
    _Use_decl_annotations_
    WSLSTATUS
    ContextBuilder::BuildContext(
        _In_ POOL_TYPE PoolType,
        _Out_ FileContext** Context
    )
    {
        *Context = nullptr;

        switch (_Type)
        {
        case FLT_FILE_CONTEXT:
            return ContextBuilder::BuildFileContext(Context, PoolType);
        default:
            return STATUS_NOT_SUPPORTED;
        }
    }
}

#endif // _CONTEXT_BUILDER_HPP_
