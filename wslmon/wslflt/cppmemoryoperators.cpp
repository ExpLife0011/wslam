#include "kernelinc.h"

#define CPP_GLOBAL_NEW_OPERATOR_TAG 'MPC!'

void *
__cdecl
operator new(
    size_t Size,
    void* Location
)
{
    UNREFERENCED_PARAMETER(Size);
    return Location;
}

_Must_inspect_result_
__drv_allocatesMem(Mem)
void*
__cdecl
operator new(
    _In_ size_t Size,
    _In_ POOL_TYPE PoolType,
    _In_ _Const_ const ULONG Tag
)
{
#ifdef DEBUG
    NT_ASSERTMS("OVERRIDE THIS!!!!", false);
#endif // DEBUG
#pragma warning(suppress:28160)
    return ::ExAllocatePoolWithTag(PoolType, Size, Tag);
}

void
__cdecl
operator delete(
    _Pre_notnull_ __drv_freesMem(Mem) void* Address,
    _In_ _Const_ const ULONG Tag
)
{
#ifdef DEBUG
    NT_ASSERTMS("OVERRIDE THIS!!!!", false);
#endif // DEBUG
    ::ExFreePoolWithTag(Address, Tag);
}

_Must_inspect_result_
__drv_allocatesMem(Mem)
void*
__cdecl
operator new(
    _In_ size_t Size
)
{
    return ::operator new(Size, PagedPool, CPP_GLOBAL_NEW_OPERATOR_TAG);
}

void
__cdecl
operator delete(
    _Pre_notnull_ __drv_freesMem(Mem) void* Address
)
{
#ifdef DEBUG
    NT_ASSERTMS("OVERRIDE THIS!!!!", false);
#endif // DEBUG
    ::operator delete(Address, (ULONG) CPP_GLOBAL_NEW_OPERATOR_TAG);
}

void
__cdecl
operator delete(
    _Pre_notnull_ __drv_freesMem(Mem) void* Address,
    _In_ size_t Size
)
{
    UNREFERENCED_PARAMETER(Size);
    ::operator delete(Address, (ULONG)CPP_GLOBAL_NEW_OPERATOR_TAG);
}

//___drv_allocatesMem(Mem)
void*
__cdecl
operator new[](
    _In_ size_t Size
)
{
#ifdef DEBUG
    NT_ASSERTMS("OVERRIDE THIS!!!!", false);
#endif // DEBUG
    return ::operator new(Size, PagedPool, CPP_GLOBAL_NEW_OPERATOR_TAG);
}

void
__cdecl
operator delete[](
    _Pre_notnull_ __drv_freesMem(Mem) void* Address
)
{
#ifdef DEBUG
    NT_ASSERTMS("OVERRIDE THIS!!!!", false);
#endif // DEBUG
    ::operator delete(Address, (ULONG)CPP_GLOBAL_NEW_OPERATOR_TAG);
}
