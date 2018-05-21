#ifndef _WSL_FILTER_HPP
#define _WSL_FILTER_HPP

#include <fltKernel.h>
#include "wsl_status.hpp"

namespace WslFlt
{

    class WslFilter
    {
    public:
        __declspec(code_seg("PAGE"))
        _Must_inspect_result_
        WSLSTATUS
        Initialize(
            _In_ PDRIVER_OBJECT DriverObject,
            _In_ PUNICODE_STRING RegistryPath
        );

        FORCEINLINE
        PFLT_FILTER
        Filter()
        {
            return _Filter;
        }

        static
        WslFilter&
        Instance()
        {
            static WslFilter s_WslFilter;
            return s_WslFilter;
        }

        __declspec(code_seg("PAGE"))
        static
        _Function_class_(PFLT_FILTER_UNLOAD_CALLBACK)
        NTSTATUS
        FLTAPI
        WslUnload(
            _In_ FLT_FILTER_UNLOAD_FLAGS Flags
        );

        __declspec(code_seg("PAGE"))
        static
        _Function_class_(PFLT_INSTANCE_SETUP_CALLBACK)
        NTSTATUS
        FLTAPI
        WslInstanceSetup(
            _In_ PCFLT_RELATED_OBJECTS FltObjects,
            _In_ FLT_INSTANCE_SETUP_FLAGS Flags,
            _In_ DEVICE_TYPE VolumeDeviceType,
            _In_ FLT_FILESYSTEM_TYPE VolumeFilesystemType
        );

        __declspec(code_seg("PAGE"))
        static
        _Function_class_(PFLT_INSTANCE_QUERY_TEARDOWN_CALLBACK)
        NTSTATUS
        FLTAPI
        WslInstanceQueryTeardown (
            _In_ PCFLT_RELATED_OBJECTS FltObjects,
            _In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
        );

        __declspec(code_seg("PAGE"))
        static
        _Function_class_(PFLT_INSTANCE_TEARDOWN_CALLBACK)
        VOID
        FLTAPI
        WslInstanceTeardownStart (
            _In_ PCFLT_RELATED_OBJECTS FltObjects,
            _In_ FLT_INSTANCE_TEARDOWN_FLAGS Flags
        );

        __declspec(code_seg("PAGE"))
        static
        _Function_class_(PFLT_INSTANCE_TEARDOWN_CALLBACK)
        VOID
        FLTAPI
        WslInstanceTeardownComplete (
            _In_ PCFLT_RELATED_OBJECTS FltObjects,
            _In_ FLT_INSTANCE_TEARDOWN_FLAGS Flags
        );

        ~WslFilter()
        {
            ::FltUnregisterFilter(_Filter);
        }

    private:
        WslFilter() = default;

    private:
        PFLT_FILTER _Filter;
        PDRIVER_OBJECT _DriverObject;
    };
}

#endif // _WSL_FILTER_HPP