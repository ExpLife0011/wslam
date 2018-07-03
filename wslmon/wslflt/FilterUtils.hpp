#ifndef _FILTER_UTILS_HPP2_
#define _FILTER_UTILS_HPP2_

#include <fltKernel.h>
#include "DynamicFunctions.hpp"
#include "wsl_status.hpp"

namespace WslFlt
{
    class FilterUtils
    {
    public:
        static
        _Must_inspect_result_
        WSLSTATUS
        OpenProcessByPid(
            _In_ _Const_ HANDLE ProcessId,
            _Out_ HANDLE * ProcessHandle
        );

        static
        _Must_inspect_result_
        bool
        IsLinuxSubsystemProcess(
            _In_ _Const_ HANDLE ProcessId
        );

        static
        _Must_inspect_result_
        WSLSTATUS
        GetProcessToken(
            _In_ PEPROCESS Process,
            _In_ ACCESS_MASK DesiredAccess,
            _Outptr_result_nullonfailure_ HANDLE* Token
        );

        static
        _Must_inspect_result_
        WSLSTATUS
        GetProcessToken(
            _In_ HANDLE Process,
            _In_ ACCESS_MASK DesiredAccess,
            _Outptr_result_nullonfailure_ HANDLE* Token
        );

        static
        _Must_inspect_result_
        WSLSTATUS
        IsTokenElevated(
            _In_ HANDLE Token,
            _Out_ bool* Elevation
        );

        static
        _Must_inspect_result_
        WSLSTATUS
        TerminateProcess(
            _In_ HANDLE ProcessId
        );

        //static
        //_Must_inspect_result_
        //WSLSTATUS
        //GetProcessElevation(
        //    _In_ PEPROCESS Process,
        //    _Out_ bool* IsElevated
        //);
    };
}

#endif // _FILTER_UTILS_HPP_
