#include <iostream>
#include <thread>
#include "DriverComm.hpp"
//#include "trace.h"
//#include "DriverComm.tmh"
#include "WslcoreExceptions.hpp"


WslCore::DriverComm::DriverComm()
    : _FilterPort{ WSL_FILTER_PORT, nullptr, 0 }
{
    _StopEvent = ::CreateEvent(nullptr, true, false, nullptr);
    if (!_StopEvent)
    {
        throw ExWin32Exception{ L"::CreateEvent failed. ", GetLastError() };
    }

    _Buffer = new BYTE[MAX_MESSAGE_LENGTH];
}


void
WslCore::DriverComm::StartCommunication()
{
    std::unique_lock<std::mutex> guard{ _Lock };
    _ListenerThread.reset(new std::thread{ &WslCore::DriverComm::Listen, this });
}


void
WslCore::DriverComm::StopCommunication()
{
    std::unique_lock<std::mutex> guard{ _Lock };
    if (_ListenerThread && _ListenerThread->joinable())
    {
        SetEvent(_StopEvent);
        CancelIo(&_FilterPort);
        _ListenerThread->join();
    }
}


void
WslCore::DriverComm::SendEcho(
    _In_ const std::string& String
)
{
    DWORD actualSize = 0;
    DWORD size = (DWORD)(String.size() + sizeof(Common::EchoMessage));
    Common::EchoMessage* message = (Common::EchoMessage*)::operator new(size);
    Common::EchoReply* reply = (Common::EchoReply*)::operator new(sizeof(Common::EchoReply) + String.size() + 1);
    message->Id = Common::MessageId::Echo;
    message->Size = size;
    CopyMemory(message->Buffer, String.c_str(), String.size());
    memset(reply, 0xee, sizeof(Common::EchoReply) + String.size() + 1);

    _FilterPort.Send(
        (PBYTE)message,
        size,
        (PBYTE)reply,
        (DWORD)(sizeof(Common::EchoReply) + String.size() + 1),
        &actualSize
    );

    reply->Buffer[String.size()] = 0;

    std::string str{(char *)reply->Buffer};
    std::cout << str << std::endl;

    ::operator delete(message);
    ::operator delete(reply);
}


WslCore::DriverComm::~DriverComm()
{
    StopCommunication();
    delete[] _Buffer;
}


void
WslCore::DriverComm::Listen()
{
    try
    {
        DispatchMessages();
    } catch (std::exception& ex)
    {
        std::cout << ex.what();
    }
}

void WslCore::DriverComm::DispatchMessages()
{
    OVERLAPPED ovlp = { 0 };
    HANDLE handles[2] = { 0 };
    DWORD waitResult = WAIT_ABANDONED;

    handles[0] = _StopEvent;
    ovlp.hEvent = ::CreateEvent(nullptr, true, false, nullptr);
    if (!ovlp.hEvent)
    {
        throw ExWin32Exception{ L"::CreateEvent failed. ", GetLastError() };
    }

    do
    {
        handles[1] = ovlp.hEvent;
        _FilterPort.Receive(reinterpret_cast<PFILTER_MESSAGE_HEADER>(_Buffer), MAX_MESSAGE_LENGTH, &ovlp);
        waitResult = ::WaitForMultipleObjects(2, handles, false, INFINITE);
        switch (waitResult)
        {
        case WAIT_OBJECT_0:
            if (!::ResetEvent(handles[0]))
            {
                throw ExWin32Exception{ L"::ResetEvent on _StopEvent failed. ", GetLastError() };
            }
            return;
        case WAIT_OBJECT_0 + 1:
            if (!::ResetEvent(handles[1]))
            {
                throw ExWin32Exception{ L"::ResetEvent on Overlapped failed. ", GetLastError() };
            }
            break;
        default:
            return;
        }
    } while (true);
}
