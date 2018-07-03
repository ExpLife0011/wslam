#ifndef _LINKED_LIST_HPP_
#define _LINKED_LIST_HPP_

#include "LookasideObject.hpp"
#include "TypeTraits.hpp"

namespace WslFlt
{
    template <typename _Ty>
    struct ListNode
    {

        LIST_ENTRY _ListEntry;
        _Ty Value;
    };
    
    template<typename _Ty>
    struct TypeTraits<ListNode<_Ty>>
    {
        static constexpr ULONG Tag = WSLFLT_TAG_LIST_NODE;
        static constexpr POOL_TYPE PoolType = TypeTraits<_Ty>::PoolType;
        static constexpr LONG NumberOfCachableElements = TypeTraits<_Ty>::NumberOfCachableElements;
    };

    template <typename _Ty>
    class LinkedList
    {
    public:
        LinkedList()
        {
            InitializeListHead(&_Head);
        }

        void
        Insert(
            _Ty Value
        )
        {
            auto node = new LookasideObject<WslFlt::ListNode<_Ty>>{};
            node->Value = Value;

            FltAcquirePushLockExclusive(&_Lock);
            InsertHeadList(&_Head, &node->_ListEntry);
            FltReleasePushLock(&_Lock);
        }

        PLIST_ENTRY
        ListHead()
        {
            return &_Head;
        }

        _Ty&
        GetObjectFromPointer(
            PLIST_ENTRY ListEntry
        )
        {
            return ((ListNode<_Ty>*) ListEntry)->Value;
        }

        ~LinkedList()
        {
            while (!IsListEmpty(&_Head))
            {
                auto node = (LookasideObject<WslFlt::ListNode<_Ty>>*) RemoveHeadList(&_Head);
                delete node;
            }
        }

    private:
        LIST_ENTRY _Head;
        EX_PUSH_LOCK _Lock;
    };
}


#endif // _LINKED_LIST_HPP_