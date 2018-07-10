#ifndef _DRIVER_COMM_HPP_
#define _DRIVER_COMM_HPP_

#include <mutex>
#include "FilterPort.hpp"

namespace WslCore
{
    class DriverComm
    {
    public:
        WSLCORE_API
        DriverComm();

        WSLCORE_API
        void
        StartCommunication();

        WSLCORE_API
        void
        StopCommunication();

        WSLCORE_API
        void
        SendEcho(
            _In_ const std::string& String
        );

        WSLCORE_API
        ~DriverComm();
    private:
        void
        Listen();

        void
        DispatchMessages();

    private:
        std::mutex _Lock;
        std::unique_ptr<std::thread> _ListenerThread;
        _Guarded_by_(_Lock) FilterPort _FilterPort;
        HANDLE _StopEvent;
        BYTE* _Buffer;
    };
}

#endif // _DRIVER_COMM_HPP_
