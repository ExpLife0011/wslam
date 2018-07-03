#ifndef _LOOKASIDE_LIST_HPP_
#define _LOOKASIDE_LIST_HPP_

#include <fltKernel.h>
#include "cppmemoryoperators.h"

namespace WslFlt
{
    template <typename T>
    class LookasideList
    {
    public:
        LookasideList(
            _In_ LONG MaximumNumberOfElements
        ) : _MaximumNumberOfElements{ MaximumNumberOfElements }
        {}

        _Must_inspect_result_
        void*
        Allocate();

        void Free(
            _In_ _Pre_notnull_ void* Object
        );

        void Flush();

        ~LookasideList()
        {
            Flush();
        }

    private:
        SLIST_HEADER _Head;
        LONG _MaximumNumberOfElements;
        __volatile volatile LONG _NumberOfFreeElements;
    };


    template<typename T>
    _Use_decl_annotations_
    void*
    WslFlt::LookasideList<T>::Allocate()
    {
        PSLIST_ENTRY entry = nullptr;

        entry = InterlockedPopEntrySList(&_Head);
        if (!entry)
        {
            return ::operator new(sizeof(T), TypeTraits<T>::PoolType, TypeTraits<T>::Tag);
        } else
        {
            InterlockedDecrement(&_NumberOfFreeElements);
        }

        return entry;
    }


    template <typename T>
    _Use_decl_annotations_
    void
    WslFlt::LookasideList<T>::Free(
        _In_ _Pre_notnull_ void* Object
    )
    {
        if (_NumberOfFreeElements <= _MaximumNumberOfElements)
        {
            InterlockedIncrement(&_NumberOfFreeElements);
            InterlockedPushEntrySList(&_Head, static_cast<PSLIST_ENTRY>(Object));
        } else
        {
            ::operator delete(Object, TypeTraits<T>::Tag);
        }
    }


    template <typename T>
    _Use_decl_annotations_
    void
    WslFlt::LookasideList<T>::Flush()
    {
        PSLIST_ENTRY pEntry = InterlockedPopEntrySList(&_Head);
        while (pEntry)
        {
            ::operator delete(pEntry, TypeTraits<T>::Tag);
            pEntry = InterlockedPopEntrySList(&_Head);
        }
    }
}

#endif // _LOOKASIDE_LIST_HPP_
