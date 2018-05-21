#ifndef _LOOKASIDE_LIST_HPP_
#define _LOOKASIDE_LIST_HPP_

#include <fltKernel.h>

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
            return new T;
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
            delete static_cast<T *>(Object);
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
            delete reinterpret_cast<T *>(pEntry);
            pEntry = InterlockedPopEntrySList(&_Head);
        }
    }
}

#endif // _LOOKASIDE_LIST_HPP_
