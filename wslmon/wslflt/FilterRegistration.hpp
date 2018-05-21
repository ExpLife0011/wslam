#ifndef _FILTER_REGISTRATION_HPP_
#define _FILTER_REGISTRATION_HPP_

#include <fltKernel.h>
#include "FileFilter.hpp"
#include "Context.hpp"
#include "WslFilter.hpp"

namespace WslFlt
{
    constexpr FLT_CONTEXT_REGISTRATION ContextRegistration[] =
    {
        {
            ContextTraits<FileContext>::Type,
            ContextTraits<FileContext>::Flags,
            ContextTraits<FileContext>::ContextCleanupCallback,
            ContextTraits<FileContext>::Size,
            TypeTraits<FileContext>::Tag,
        },
        {
            ContextTraits<StreamContext>::Type,
            ContextTraits<StreamContext>::Flags,
            ContextTraits<StreamContext>::ContextCleanupCallback,
            ContextTraits<StreamContext>::Size,
            TypeTraits<StreamContext>::Tag,
        },
        {
            ContextTraits<StreamHandleContext>::Type,
            ContextTraits<StreamHandleContext>::Flags,
            ContextTraits<StreamHandleContext>::ContextCleanupCallback,
            ContextTraits<StreamHandleContext>::Size,
            TypeTraits<StreamHandleContext>::Tag,
        },
        { FLT_CONTEXT_END }
    };

    constexpr FLT_OPERATION_REGISTRATION OperationRegistration[] =
    {
        {
            IRP_MJ_CREATE,
            0,
            WslFlt::FileFilter::WslPreCreate,
            WslFlt::FileFilter::WslPostCreate
        },
        { IRP_MJ_OPERATION_END }
    };

    constexpr FLT_REGISTRATION FilterRegistration =
    {
        sizeof(FLT_REGISTRATION),
        FLT_REGISTRATION_VERSION,
        0,
        ContextRegistration,
        OperationRegistration,
        WslFilter::WslUnload,
        WslFilter::WslInstanceSetup,
        WslFilter::WslInstanceQueryTeardown,
        WslFilter::WslInstanceTeardownStart,
        WslFilter::WslInstanceTeardownComplete,
        nullptr,
        nullptr,
        nullptr
    };
}

#endif // _FILTER_REGISTRATION_HPP_