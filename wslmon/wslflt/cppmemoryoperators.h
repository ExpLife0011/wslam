#ifndef _CPPMEMORYOPERATORS_H_
#define _CPPMEMORYOPERATORS_H_

#include "kernelinc.h"

#ifdef _WIN64
typedef unsigned __int64 size_t;
#else
typedef unsigned int size_t;
#endif //AMD64

_Must_inspect_result_
__drv_allocatesMem(Mem)
void*
__cdecl
operator new(
    _In_ size_t Size,
    _In_ POOL_TYPE PoolType,
    _In_ _Const_ const ULONG Tag
);

void
__cdecl
operator delete(
    _Pre_notnull_ __drv_freesMem(Mem) void* Address,
    _In_ _Const_ const ULONG Tag
);
#endif // !_CPPMEMORYOPERATORS_H_

