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
    };
}

#endif // _FILTER_UTILS_HPP_
