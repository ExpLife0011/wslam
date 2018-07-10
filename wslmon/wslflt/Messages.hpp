#ifndef _MESSAGES_HPP_
#define _MESSAGES_HPP_

#include "wsl_status.hpp"

#define MAX_MESSAGE_LENGTH 4 * 1024 * 1024
#define WSL_FILTER_PORT L"\\WslFlt"

#pragma warning(push)
#pragma warning(disable: 4200) // C4200: nonstandard extension used: zero-sized array in struct/union

#pragma pack(push)
#pragma pack(1)

typedef unsigned char BYTE, *PBYTE;

namespace Common
{
    enum class MessageId : int
    {
        StartMonitoring,
        StopMonitoring,
        __LastMsg,
        Echo = __LastMsg
    };

    struct MessageHeader
    {
        MessageId Id;
        size_t Size;
    };

    struct ReplyHeader
    {
        MessageHeader Header;
        WSLSTATUS Status;
    };

    struct EchoMessage
        : public MessageHeader
    {
        BYTE Buffer[0];
    };

    struct EchoReply
        : public ReplyHeader
    {
        BYTE Buffer[0];
    };
}

#pragma pack(pop)
#pragma warning(pop)

#endif // _MESSAGES_HPP_