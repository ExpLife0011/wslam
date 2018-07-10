#ifndef _FILE_FILTER_HPP_
#define _FILE_FILTER_HPP_

#include <fltKernel.h>
#include "Context.hpp"
#include "LookasideObject.hpp"
#include "FltPort.hpp"


namespace WslFlt
{
    class FileFilter
    {
    public:
        static
        __forceinline
        bool
        IsMonitoring()
        {
            return _State == (LONG)FileFilterState::started;
        }

        static
        __forceinline
        void
        StartMonitoring()
        {
            InterlockedCompareExchange(&_State, (LONG)FileFilterState::started, (LONG)FileFilterState::stopped);
        }

        static
        __forceinline
        void
        StopMonitoring()
        {
            InterlockedCompareExchange(&_State, (LONG)FileFilterState::stopped, (LONG)FileFilterState::started);
        }

        static
        _Function_class_(PFLT_PRE_OPERATION_CALLBACK)
        FLT_PREOP_CALLBACK_STATUS
        WslPreCreate(
            _Inout_ PFLT_CALLBACK_DATA Cbd,
            _In_ PCFLT_RELATED_OBJECTS FltObjects,
            _Flt_CompletionContext_Outptr_ PVOID *CompletionContext
        );

        static
        _Function_class_(PFLT_POST_OPERATION_CALLBACK)
        FLT_POSTOP_CALLBACK_STATUS
        WslPostCreate(
            _Inout_ PFLT_CALLBACK_DATA Data,
            _In_ PCFLT_RELATED_OBJECTS FltObjects,
            _In_opt_ PVOID CompletionContext,
            _In_ FLT_POST_OPERATION_FLAGS Flags
        );

        static
        _Function_class_(PFLT_PRE_OPERATION_CALLBACK)
        FLT_PREOP_CALLBACK_STATUS
        WslPreRead(
            _Inout_ PFLT_CALLBACK_DATA Cbd,
            _In_ PCFLT_RELATED_OBJECTS FltObjects,
            _Flt_CompletionContext_Outptr_ PVOID *CompletionContext
        );

        static
        _Function_class_(PFLT_POST_OPERATION_CALLBACK)
        FLT_POSTOP_CALLBACK_STATUS
        WslPostRead(
            _Inout_ PFLT_CALLBACK_DATA Data,
            _In_ PCFLT_RELATED_OBJECTS FltObjects,
            _In_opt_ PVOID CompletionContext,
            _In_ FLT_POST_OPERATION_FLAGS Flags
        );

        static FltPort _Port;
    private:
        static
        _Must_inspect_result_
        LookasideObject<WslCompletionContext>*
        CreateCompletionContext(
            _In_ PFLT_CALLBACK_DATA Data
        );

    private:
        enum class FileFilterState
        {
            stopped,
            started,
        };

        static __volatile volatile LONG _State;
    };
}

#endif // _FILE_FILTER_HPP_
