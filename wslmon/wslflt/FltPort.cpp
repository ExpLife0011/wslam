#include "Messages.hpp"
#include "FltPort.hpp"
#include "wslflt_trace.h"
#include "FltPort.tmh"

using namespace WslFlt;

_Use_decl_annotations_
void
WslFlt::FltPortClient::Connect(
    _In_ WslFlt::FltPort* Owner,
    _In_ PFLT_FILTER Filter,
    _In_ PFLT_PORT Port
)
{
    _Owner = Owner;
    _Filter = Filter;
    _Port = Port;
}


void WslFlt::FltPortClient::Disconnect()
{
    if (_Filter && _Port)
    {
        ::FltCloseClientPort(_Filter, &_Port);
        _Filter = nullptr;
        _Port = nullptr;
    }
}


WslFlt::FltPortClient::~FltPortClient()
{
    Disconnect();
}


_Use_decl_annotations_
FltPort::FltPort(
    _In_ _Const_ LPCWSTR PortName,
    _In_ PFLT_FILTER Filter,
    _In_ LONG NumberOfConnections
) : _PortState{ stateUninitialized }, _Filter{ Filter }, _Port{ nullptr }, _FltPortClient{}, _IsValid{ false }
{
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    OBJECT_ATTRIBUTES oa = { 0 };
    UNICODE_STRING ustrPortName = { 0 };
    PSECURITY_DESCRIPTOR sd = nullptr;

    RtlInitUnicodeString(&ustrPortName, PortName);

    WslLogTrace("FltBuildDefaultSecurityDescriptor DesiredAccess=0x%X", FLT_PORT_ALL_ACCESS);
    status = ::FltBuildDefaultSecurityDescriptor(&sd, FLT_PORT_ALL_ACCESS);
    if (!NT_SUCCESS(status))
    {
        WslLogError("FltBuildDefaultSecurityDescriptor failed with status: 0x%X", status);
        goto CleanUp;
    }

    InitializeObjectAttributes(&oa,
        &ustrPortName,
        OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
        NULL,
        sd
    );

    WslLogTrace("FltCreateCommunicationPort Port=%wZ MaxConnections=0x%X", &ustrPortName, NumberOfConnections);
    status = ::FltCreateCommunicationPort(
        Filter,
        &_Port,
        &oa,
        this,
        &FltPort::OnConnect,
        &FltPort::OnDisconnect,
        &FltPort::OnMessageReceived,
        NumberOfConnections
    );
    if (!NT_SUCCESS(status))
    {
        WslLogError("FltCreateCommunicationPort failed with status: 0x%X", status);
        goto CleanUp;
    }

    WslLogInfo("Opened port handle 0x%p", _Port);
    _IsValid = true;

CleanUp:
    if (sd)
    {
        ::FltFreeSecurityDescriptor(sd);
    }

    _PortState = stateInitialized;
}


_Use_decl_annotations_
NTSTATUS
WslFlt::FltPort::Dispatch(
    _In_opt_ FltPortClient* Client,
    _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
    _In_ ULONG InputBufferLength,
    _Out_writes_bytes_to_opt_(OutputBufferLength, *ReturnOutputBufferLength) PVOID OutputBuffer,
    _In_ ULONG OutputBufferLength,
    _Out_ PULONG ReturnOutputBufferLength
)
{
    Common::MessageHeader* header = (Common::MessageHeader*)InputBuffer;

    UNREFERENCED_PARAMETER(Client);
    UNREFERENCED_PARAMETER(InputBufferLength);

    *ReturnOutputBufferLength = 0;

    switch (header->Id)
    {
    case Common::MessageId::Echo:
    {
        Common::EchoMessage* message= (Common::EchoMessage*) InputBuffer;
        Common::EchoReply* reply = (Common::EchoReply*) OutputBuffer;
        reply->Header = *header;
        reply->Status = STATUS_SUCCESS;
        RtlCopyMemory(reply->Buffer, message->Buffer, (header->Size - sizeof(Common::MessageHeader)));
        *ReturnOutputBufferLength = OutputBufferLength;
    }
    }

    return STATUS_SUCCESS;
}

FltPort::~FltPort()
{
    WslLogInfo("Closing port handle 0x%p", _Port);
    ::FltClose(_Port);
    _Port = nullptr;
}


_Function_class_(PFLT_CONNECT_NOTIFY)
NTSTATUS
__stdcall
FltPort::OnConnect(
    _In_ PFLT_PORT ClientPort,
    _In_opt_ PVOID ServerPortCookie,
    _In_reads_bytes_opt_(SizeOfContext) PVOID ConnectionContext,
    _In_ ULONG SizeOfContext,
    _Outptr_result_maybenull_ PVOID *ConnectionPortCookie
)
{
    UNREFERENCED_PARAMETER(ConnectionContext);
    UNREFERENCED_PARAMETER(SizeOfContext);

    *ConnectionPortCookie = nullptr;
    if (!ServerPortCookie)
    {
        return STATUS_SUCCESS;
    }

    FltPort* thisPort = reinterpret_cast<FltPort *>(ServerPortCookie);
    thisPort->_FltPortClient.Connect(thisPort, thisPort->_Filter, ClientPort);
    *ConnectionPortCookie = &thisPort->_FltPortClient;

    return STATUS_SUCCESS;
}


_Function_class_(PFLT_DISCONNECT_NOTIFY)
void
__stdcall
FltPort::OnDisconnect(
    _In_opt_ PVOID ConnectionCookie
)
{
    FltPortClient *client = reinterpret_cast<FltPortClient *>(ConnectionCookie);
    client->Disconnect();
}


_Function_class_(PFLT_MESSAGE_NOTIFY)
NTSTATUS
__stdcall
FltPort::OnMessageReceived(
    _In_opt_ PVOID PortCookie,
    _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
    _In_ ULONG InputBufferLength,
    _Out_writes_bytes_to_opt_(OutputBufferLength, *ReturnOutputBufferLength) PVOID OutputBuffer,
    _In_ ULONG OutputBufferLength,
    _Out_ PULONG ReturnOutputBufferLength
)
{
    FltPortClient* client = reinterpret_cast<FltPortClient*>(PortCookie);
    return client->_Owner->Dispatch(client, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength, ReturnOutputBufferLength);
}
