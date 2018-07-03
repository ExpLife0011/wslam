#ifndef _PROCESS_COLLECTOR_HPP_
#define _PROCESS_COLLECTOR_HPP_

#include <fltKernel.h>
#include "wsl_status.hpp"
#include "Process.hpp"
#include "SharedPointer.hpp"
#include "LinkedList.hpp"

namespace WslFlt
{
    class ProcessCollector
    {
    public:
        static
        ProcessCollector&
        Instance();

        _Must_inspect_result_
        WSLSTATUS
        InsertProcess(
            _In_ HANDLE ProcessId,
            _In_ HANDLE Token,
            _In_ bool IsElevated
        );

        _Must_inspect_result_
        WSLSTATUS
        GetProcessByPid(
            _In_ HANDLE ProcessId,
            _Out_ WslFlt::SharedPointer<WslFlt::Process>& Process
        );


    private:
        ProcessCollector();
        LinkedList<WslFlt::SharedPointer<WslFlt::LookasideObject<WslFlt::Process>>> _Processes;

    };
}

#endif // _PROCESS_COLLECTOR_HPP_
