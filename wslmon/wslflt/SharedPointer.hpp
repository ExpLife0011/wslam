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


        template <typename ... Args>
        SharedPointer(
            Args&& ... Arguments
        )
        {
            void* mem = ::operator new(sizeof(Refcounter) + sizeof(_Ty), Traits::PoolType, Traits::Tag);
            _Refcounter = ::new(mem) Refcounter();
            _Obj = ::new(static_cast<char *>(mem) + sizeof(Refcounter)) _Ty{ WslFlt::Forward<Args>(Arguments) ... };
            _DeleteBoth = true;
            
        }

        constexpr
        SharedPointer(){}

        SharedPointer(
            _In_ _Ty* Object
        )
        {
            _Refcounter = new Refcounter{};
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
            Other.Reference();
            _Obj = reinterpret_cast<_Ty>(Other->GetWeakReference());
            _Refcounter = Other._Refcounter;
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

    private:
        void
        Reference(
            _In_ SharedPointer<_Ty>& Pointer
        )
        {
            if (Pointer._Refcounter)
            {
                Pointer._Refcounter->Reference();
                _Obj = Pointer.GetWeakReference();
                _Refcounter = Pointer._Refcounter;
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
                delete _Refcounter;
                delete _Obj;
            }
        }

        Refcounter* _Refcounter;
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
        return SharedPointer<Type>(static_cast<Args&&>(Arguments) ...);
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


