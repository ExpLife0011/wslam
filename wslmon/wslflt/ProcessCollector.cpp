#include "ProcessCollector.hpp"
#include "LookasideObject.hpp"


WslFlt::ProcessCollector&
WslFlt::ProcessCollector::Instance()
{
    static ProcessCollector s_ProcessCollector;
    return s_ProcessCollector;
}

_Use_decl_annotations_
WSLSTATUS
WslFlt::ProcessCollector::InsertProcess(
    _In_ HANDLE ProcessId,
    _In_ HANDLE Token,
    _In_ bool IsElevated
)
{
    auto pProcess = WslFlt::SharedPointer<WslFlt::LookasideObject<WslFlt::Process>>{ new WslFlt::LookasideObject<WslFlt::Process>{ ProcessId, Token, IsElevated } };
    _Processes.Insert(pProcess);
    return STATUS_SUCCESS;
}

_Use_decl_annotations_
WSLSTATUS
WslFlt::ProcessCollector::GetProcessByPid(
    _In_ HANDLE ProcessId,
    _Out_ WslFlt::SharedPointer<WslFlt::Process>& Process
)
{
    auto node = _Processes.ListHead()->Flink;
    while (node != _Processes.ListHead())
    {
        auto proc = _Processes.GetObjectFromPointer(node);
        if (ProcessId == proc->ProcessId())
        {
            Process = proc;
            return STATUS_SUCCESS;
        }
        node = node->Flink;
    }
    return STATUS_NOT_FOUND;
    
}


WslFlt::ProcessCollector::ProcessCollector()
{}
