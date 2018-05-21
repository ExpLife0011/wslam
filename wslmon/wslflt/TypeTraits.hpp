#ifndef _TYPE_TRAITS_HPP_
#define _TYPE_TRAITS_HPP_

#include "kernelinc.h"
#include "FilterTags.hpp"

#define DEFAULT_NUMBER_OF_CACHABLE_ELEMENTS 10

namespace WslFlt
{
    template<class _Ty>
    struct RemoveReference
    {
        using Type = _Ty;
    };

    template<class _Ty>
    struct RemoveReference<_Ty&>
    {
        using Type = _Ty;
    };

    template<class _Ty>
    struct RemoveReference<_Ty&&>
    {
        using Type = _Ty;
    };

    template<class _Ty>
    using RemoveReferenceType = typename RemoveReference<_Ty>::Type;

    template <typename _Ty>
    struct TypeTraits
    {
        static constexpr ULONG Tag = LIB_TAG_DEFAULT;
        static constexpr POOL_TYPE PoolType = PagedPool;
        static constexpr LONG NumberOfCachableElements = DEFAULT_NUMBER_OF_CACHABLE_ELEMENTS;
    };

    template<class _Ty>
    constexpr _Ty&& Forward(RemoveReferenceType<_Ty>& _Arg)
    {
        return (static_cast<_Ty&&>(_Arg));
    }
}

#endif // _TYPE_TRAITS_HPP_
