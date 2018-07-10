#ifndef _TEST_HEADER_HPP_
#define _TEST_HEADER_HPP_

#include "wslcore.h"
#define WIN32_NO_STATUS
#include <Windows.h>
#undef WIN32_NO_STATUS

namespace WslCore
{
    class TS
    {
    public:
        WSLCORE_API
            TS(int X);

        WSLCORE_API
        int GetX()
        {
            return _X;
        }

        WSLCORE_API
            void SetX(int X)
        {
            _X = X;
        }

        WSLCORE_API
            TS(float id);

        WSLCORE_API
        TS(const wchar_t* PortName);

        WSLCORE_API
        TS(
            LPCWSTR PortName,
            LPCVOID Context,
            WORD ContextSize
        );

    private:
        int _X;
        HANDLE _Port;
        OVERLAPPED _Ovlp;
    };
}

#endif // _TEST_HEADER_HPP_
