#ifndef _FLT_PORT_HPP_
#define _FLT_PORT_HPP_

#include <fltKernel.h>
#include "State.hpp"

namespace WslFlt
{
    class FltPortClient
    {
        friend class FltPort;

    public:
        FltPortClient()
            : _Owner { nullptr }, _Filter{ nullptr }, _Port{ nullptr }
        {}

        FltPortClient(
            _In_ FltPort* Owner,
            _In_ PFLT_FILTER Filter,
            _In_ PFLT_PORT Port
        ) : _Owner { Owner }, _Filter{ Filter }, _Port{ Port }
        {}

        void
        Connect(
            _In_ FltPort* Owner,
            _In_ PFLT_FILTER Filter,
            _In_ PFLT_PORT Port
        );

        void
        Disconnect();

        ~FltPortClient();
    private:
        FltPort* _Owner;
        PFLT_FILTER _Filter;
        PFLT_PORT _Port;
    };


    class FltPort
    {
    public:
        void
        Connect(
            _In_ _Const_ LPCWSTR PortName,
            _In_ PFLT_FILTER Filter,
            _In_ LONG NumberOfConnections
        );
        
        _Must_inspect_result_
        FORCEINLINE
        bool
        IsValid() const
        {
            return _IsValid;
        }

        _Must_inspect_result_
        NTSTATUS
        Dispatch(
            _In_opt_ FltPortClient* Client,
            _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
            _In_ ULONG InputBufferLength,
            _Out_writes_bytes_to_opt_(OutputBufferLength, *ReturnOutputBufferLength) PVOID OutputBuffer,
            _In_ ULONG OutputBufferLength,
            _Out_ PULONG ReturnOutputBufferLength
        );

        void
        SendDetection(
            PUNICODE_STRING Name
        );

        ~FltPort();

    private:
        _Function_class_(PFLT_CONNECT_NOTIFY)
        static
        NTSTATUS
        __stdcall
        OnConnect(
            _In_ PFLT_PORT ClientPort,
            _In_opt_ PVOID ServerPortCookie,
            _In_reads_bytes_opt_(SizeOfContext) PVOID ConnectionContext,
            _In_ ULONG SizeOfContext,
            _Outptr_result_maybenull_ PVOID *ConnectionPortCookie
        );

        _Function_class_(PFLT_DISCONNECT_NOTIFY)
        static
        void
        __stdcall
        OnDisconnect(
            _In_opt_ PVOID ConnectionCookie
        );

        _Function_class_(PFLT_MESSAGE_NOTIFY)
        static
        NTSTATUS
        __stdcall
        OnMessageReceived(
            _In_opt_ PVOID PortCookie,
            _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
            _In_ ULONG InputBufferLength,
            _Out_writes_bytes_to_opt_(OutputBufferLength, *ReturnOutputBufferLength) PVOID OutputBuffer,
            _In_ ULONG OutputBufferLength,
            _Out_ PULONG ReturnOutputBufferLength
        );

    private:
        __interlocked STATE _PortState;
        PFLT_FILTER _Filter;
        PFLT_PORT _Port;
        FltPortClient _FltPortClient;
        bool _IsValid;
    };
}

#endif // _FLT_PORT_HPP_
