#ifndef _PROCESS_FILTER_HPP
#define _PROCESS_FILTER_HPP

#include <fltKernel.h>
#include "wsl_status.hpp"

namespace WslFlt
{
    class ProcessFilter
    {
    public:
        static
        ProcessFilter&
        Instance()
        {
            static ProcessFilter s_ProcessFilter;
            return s_ProcessFilter;
        }

        _Must_inspect_result_
        _IRQL_requires_max_(PASSIVE_LEVEL)
        WSLSTATUS
        StartProcessFiltering();

        _Must_inspect_result_
        _IRQL_requires_max_(PASSIVE_LEVEL)
        WSLSTATUS
        StopProcessFiltering();

        _Must_inspect_result_
        _IRQL_requires_max_(PASSIVE_LEVEL)
        WSLSTATUS
        StartThreadFiltering();

        _Must_inspect_result_
        _IRQL_requires_max_(PASSIVE_LEVEL)
        WSLSTATUS
        StopThreadFiltering();

    private:
        ProcessFilter() = default;
    };
}
#endif // _PROCESS_FILTER_HPP

