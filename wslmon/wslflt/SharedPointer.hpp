#ifndef _SHARED_POINTER_HPP_
#define _SHARED_POINTER_HPP_

#include <sal.h>
#include "Refcounter.hpp"
#include "cppmemoryoperators.h" 
#include "TypeTraits.hpp"

namespace WslFlt
{
    template <typename _Ty>
    class SharedPointer
    {
    public:
        using Type          = _Ty;
        using PointerType   = _Ty*;
        using Traits        = TypeTraits<Type>;

        template<class _OhterType>
        friend class SharedPointer;


        template <typename ... Args>
        SharedPointer(
            Args&& ... Arguments
        )
        {
            void* mem = ::operator new(sizeof(Refcounter) + sizeof(_Ty), Traits::PoolType, Traits::Tag);
            _Obj = new(static_cast<char *>(mem)) _Ty{ WslFlt::Forward<Args>(Arguments) ... };
            _Refcounter = new(mem + sizeof(_Ty)) Refcounter();
            _DeleteBoth = true;
            
        }

        SharedPointer()
            : _Obj{ nullptr }, _Refcounter{ nullptr }, _DeleteBoth{ false }
        {}

        SharedPointer(
            _In_ _Ty* Object
        )
        {
            auto rfc = ::operator new(sizeof(Refcounter), TypeTraits<_Ty>::PoolType, TypeTraits<Refcounter>::Tag);
            _Refcounter = new((void*)rfc) Refcounter {};
            _Obj = Object;
            _DeleteBoth = false;
        }

        SharedPointer(
            _In_ const SharedPointer<_Ty>& Other
        )
        {
            Reset(Other);
        }

        SharedPointer(
            _In_ SharedPointer<_Ty>&& Other
        )
        {
            Reset(Other);
            Other.Release();
        }

        template <typename _OtherType>
        SharedPointer(
            _In_ SharedPointer<_OtherType>& Other
        )
        {
            Reference(Other);
            //_Obj = reinterpret_cast<_Ty*>(Other.GetWeakReference());
            //_Refcounter = Other._Refcounter;
        }

        void
        Release()
        {
            if (!_Refcounter)
            {
                return;
            }
            if (0 == _Refcounter->DereferenceAndGetRefcount())
            {
                Destroy();
            }
        }

        void
        Reset()
        {
            Release();
            _Obj = nullptr;
        }

        void
        Reset(
            _In_ SharedPointer<_Ty>& Other
        )
        {
            Release();
            Reference(Other);
        }

        SharedPointer<_Ty>& operator=(
            _In_ SharedPointer<_Ty>& Other
        )
        {
            Reference(Other);
            return *this;
        }

        SharedPointer<_Ty>& operator=(
            _In_ SharedPointer<_Ty>&& Other
        )
        {
            Reference(Other);
            return *this;
        }

        _Ty*
        GetWeakReference()
        {
            return _Obj;
        }

        FORCEINLINE
        explicit
        operator bool() const
        {
            return (GetWeakReference() != nullptr);
        }

        template <typename _Ty2>
        bool
        operator==(const SharedPointer<_Ty2> Other)
        {
            return (PVOID)GetWeakReference() == (PVOID)Other->GetWeakReference();
        }

        FORCEINLINE
        _Ty* 
        operator->()
        {
            return GetWeakReference();
        }

        FORCEINLINE
        _Ty*
        operator*()
        {
            return GetWeakReference();
        }

        ~SharedPointer()
        {
            Release();
        }

        void
        IncrementReferences()
        {
            _Refcounter->Reference();
        }

    private:
        template <typename _OtherType>
        void
        Reference(
            _In_ SharedPointer<_OtherType>& Pointer
        )
        {
            if (Pointer._Refcounter)
            {
                Pointer._Refcounter->Reference();
                _Obj = Pointer.GetWeakReference();
                _Refcounter = Pointer._Refcounter;
                _DeleteBoth = Pointer._DeleteBoth;
            }
        }


        void
        Destroy()
        {
            if (_DeleteBoth)
            {
                _Refcounter->~Refcounter();
                _Obj->~_Ty();
                ::operator delete((void*)_Refcounter, Traits::Tag);
            }
            else
            {
                ::operator delete((void *)_Refcounter, TypeTraits<Refcounter>::Tag);
                delete _Obj;
            }
        }

        mutable Refcounter* _Refcounter;
        _Ty* _Obj;
        bool _DeleteBoth;
    };


    template <typename Type, typename ... Args>
    FORCEINLINE
    SharedPointer<Type>
    MakeShared(
        Args&& ... Arguments
    )
    {
        return SharedPointer<Type>(WslFlt::Forward<Args>(Arguments) ...);
    }

    template<typename _Ty1, typename _Ty2>
    FORCEINLINE
    SharedPointer<_Ty2>
    PointerCast(SharedPointer<_Ty1> Pointer)
    {
        const auto _Ptr = reinterpret_cast<_Ty2>(Pointer.GetWeakReference());
        return SharedPointer<_Ty2>{ Pointer, _Ptr };
    }
}

#endif // _SHARED_POINTER_HPP_


