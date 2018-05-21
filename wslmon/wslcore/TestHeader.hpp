#ifndef _TEST_HEADER_HPP_
#define _TEST_HEADER_HPP_

#include "wslcore.h"

namespace WslCore
{
    class TS
    {
    public:
        WSLCORE_API
        TS(int X) : _X{ X } {}

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

    private:
        int _X;
    };
}

#endif // _TEST_HEADER_HPP_
