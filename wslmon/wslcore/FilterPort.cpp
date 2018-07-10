#include "FilterPort.hpp"
#include "wslcore_trace.h"
#include "FilterPort.cpp.tmh"
#include "WslcoreExceptions.hpp"

_Use_decl_annotations_
WslCore::FilterPort::FilterPort(
    _In_ LPCWSTR PortName,
    _In_opt_ LPCVOID Context,
    _In_opt_ WORD ContextSize
)
{
    HRESULT hRes = E_UNEXPECTED;
    WslCoreLogTrace("::FilterConnectCommunicationPort(%S, 0, 0x%p, 0x%x nullptr, 0x%p).", PortName, Context, ContextSize, &_Port);
    hRes = ::FilterConnectCommunicationPort(
        PortName, 
        0, 
        Context, 
        ContextSize, 
        nullptr, 
        &_Port
    );
    if (!SUCCEEDED(hRes))
    {
        WslCoreLogError("::FilterConnectCommunicationPort(%S, 0, 0x%p, 0x%x nullptr, 0x%p) failed. HRESULT=0x%x.", PortName, Context, ContextSize, &_Port, hRes);
        throw ExWin32Exception{ L"::FilterConnectCommunicationPort failed. ", hRes };
    }
}

_Use_decl_annotations_
void
WslCore::FilterPort::Send(
    _In_ _Readable_bytes_(MessageBufferSize) BYTE* MessageBuffer,
    _In_ DWORD MessageBufferSize,
    _Out_writes_bytes_to_opt_(ReplyBufferSize, *ActualReplyBufferSize) BYTE* ReplyBuffer,
    _In_ DWORD ReplyBufferSize,
    _Out_ LPDWORD ActualReplyBufferSize
)
{
    HRESULT hRes = E_UNEXPECTED;
    WslCoreLogTrace("::FilterSendMessage(0x%p, 0x%p, 0x%x, 0x%p, 0x%x, 0x%p)", _Port, MessageBuffer, MessageBufferSize, ReplyBuffer, ReplyBufferSize, ActualReplyBufferSize);
    hRes = ::FilterSendMessage(
        _Port,
        MessageBuffer,
        MessageBufferSize,
        ReplyBuffer,
        ReplyBufferSize,
        ActualReplyBufferSize
    );
    if (!SUCCEEDED(hRes))
    {
        WslCoreLogError("::FilterSendMessage(0x%p, 0x%p, 0x%x, 0x%p, 0x%x, 0x%p) failed. HRESULT=0x%x.", _Port, MessageBuffer, MessageBufferSize, ReplyBuffer, ReplyBufferSize, ActualReplyBufferSize, hRes);
        throw ExWin32Exception{ L"::FilterSendMessage failed. ", hRes };
    }
}


_Use_decl_annotations_
void
WslCore::FilterPort::Send(
    _In_ _Readable_bytes_(MessageBufferSize) BYTE* MessageBuffer,
    _In_ DWORD MessageBufferSize
)
{
    DWORD bytesWritten = 0;
    Send(MessageBuffer, MessageBufferSize, nullptr, 0, &bytesWritten);
}

_Use_decl_annotations_
void
WslCore::FilterPort::Receive(
    _Out_writes_bytes_(MessageBufferSize) PFILTER_MESSAGE_HEADER MessageBuffer,
    _In_ DWORD MessageBufferSize,
    _Inout_opt_ LPOVERLAPPED Overlapped
)
{
    HRESULT hRes = E_UNEXPECTED;
#pragma warning(suppress: 28193)
    hRes = ::FilterGetMessage(
        _Port,
        (PFILTER_MESSAGE_HEADER)MessageBuffer,
        MessageBufferSize,
        Overlapped
    );
    if (IS_ERROR(hRes))
    {
        WslCoreLogError("::FilterGetMessage(0x%p, 0x%p, 0x%x, 0x%p) returned 0x%x.", _Port, MessageBuffer, MessageBufferSize, Overlapped, hRes);
        throw WslCoreException{ L"::FilterGetMessage ", GetLastError() };
    }
}

_Use_decl_annotations_
void
WslCore::FilterPort::Reply(
    _In_ _Readable_bytes_(ReplyBufferSize) PFILTER_REPLY_HEADER ReplyBuffer,
    _In_ DWORD ReplyBufferSize
)
{
    HRESULT hRes = E_UNEXPECTED;

    WslCoreLogTrace("::FilterReplyMessage(0x%p, 0x%p, 0x%x)", _Port, ReplyBuffer, ReplyBufferSize);
    hRes = ::FilterReplyMessage(
        _Port,
        (PFILTER_REPLY_HEADER)ReplyBuffer,
        ReplyBufferSize
    );
    if (!SUCCEEDED(hRes))
    {
        WslCoreLogError("::FilterReplyMessage(0x%p, 0x%p, 0x%x) failed. HRESULT=0x%x", _Port, ReplyBuffer, ReplyBufferSize, hRes);
        throw ExWin32Exception{ L"::FilterReplyMessage failed", hRes };
    }
}

WslCore::FilterPort::~FilterPort()
{
    ::CloseHandle(_Port);
    _Port = nullptr;
}
