#include "TestHeader.hpp"
#include "wslcore_trace.h"
#include "TestHeader.cpp.tmh"
#include "WslcoreExceptions.hpp"
#include <fltUser.h>

WslCore::TS::TS(int X)
{
    _X = X;
}

WslCore::TS::TS(LPCWSTR PortName, LPCVOID Context, WORD ContextSize)
{
    PortName;
    Context;
    ContextSize;

}

WslCore::TS::TS(float id)
{
    _X = (int)id;
}

WslCore::TS::TS(const wchar_t* PortName)
{
    PortName;
    _X = 1;
}
