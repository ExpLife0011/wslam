#ifndef _REFCOUNTER_HPP_
#define _REFCOUNTER_HPP_

#include "TypeTraits.hpp"

namespace WslFlt
{
    class Refcounter
    {
        template <typename _Ty>
        friend class SharedPointer;
    public:
        Refcounter() : _Refcount{ 1 }
        {}

        void
        Reference();

        long
        DereferenceAndGetRefcount();

        long
        GetRefcount();

        ~Refcounter() = default;
    private:
        volatile long _Refcount;
    };

    template<>
    struct TypeTraits<Refcounter>
    {
        static constexpr ULONG Tag = WSLFLT_TAG_RFC;
        static constexpr POOL_TYPE PoolType = PagedPool;
        static constexpr LONG NumberOfCachableElements = DEFAULT_NUMBER_OF_CACHABLE_ELEMENTS;
    };
}

#endif // _REFCOUNTER_HPP_
