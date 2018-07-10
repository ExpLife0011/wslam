#ifndef _FILTER_PORT_HPP_
#define _FILTER_PORT_HPP_

#include <memory>
#define WIN32_NO_STATUS
#include <Windows.h>
#undef WIN32_NO_STATUS
#include <fltUser.h>
#include "wslcore.h"
#include "Messages.hpp"

namespace WslCore
{
    class FilterPort
    {
    public:
        WSLCORE_API
        FilterPort(
            _In_ LPCWSTR PortName,
            _In_opt_ LPCVOID Context,
            _In_opt_ WORD ContextSize
        );

        WSLCORE_API
        void
        Send(
            _In_ _Readable_bytes_(MessageBufferSize) BYTE* MessageBuffer,
            _In_ DWORD MessageBufferSize,
            _Out_writes_bytes_to_opt_(ReplyBufferSize, *ActualReplyBufferSize) BYTE* ReplyBuffer,
            _In_ DWORD ReplyBufferSize,
            _Out_ LPDWORD ActualReplyBufferSize
        );

        WSLCORE_API
        void
        Send(
            _In_ _Readable_bytes_(MessageBufferSize) BYTE* MessageBuffer,
            _In_ DWORD MessageBufferSize
        );

        WSLCORE_API
        _Pre_satisfies_(MessageBufferSize >= sizeof(FILTER_MESSAGE_HEADER))
        void
        Receive(
            _Out_writes_bytes_(MessageBufferSize) PFILTER_MESSAGE_HEADER MessageBuffer,
            _In_ DWORD MessageBufferSize,
            _Inout_opt_ LPOVERLAPPED Overlapped
        );

        WSLCORE_API
        _Pre_satisfies_(ReplyBufferSize >= sizeof(FILTER_REPLY_HEADER))
        void
        Reply(
            _In_ _Readable_bytes_(ReplyBufferSize) PFILTER_REPLY_HEADER ReplyBuffer,
            _In_ DWORD ReplyBufferSize
        );

        WSLCORE_API
            ~FilterPort();
    private:
        HANDLE _Port;
        OVERLAPPED _Ovlp;
        HANDLE _Handles[2];
    };
}

#endif // _FILTER_PORT_HPP_

