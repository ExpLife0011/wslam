#include "kernelinc.h"
#include "Refcounter.hpp"

void
WslFlt::Refcounter::Reference()
{
    long local = 0;
    do
    {
        local = _Refcount;
        if (0 >= local)
        {
            __debugbreak();
            return;
        }
    } while (local != _InterlockedCompareExchange(&_Refcount, local + 1, local));
}


LONG
WslFlt::Refcounter::DereferenceAndGetRefcount()
{
    long local = 0;
    do
    {
        local = _Refcount;
        if (0 >= local)
        {
            __debugbreak();
            return local;
        }
    } while (local != _InterlockedCompareExchange(&_Refcount, local - 1, local));

    return local - 1;
}


long
WslFlt::Refcounter::GetRefcount()
{
    return _Refcount;
}
