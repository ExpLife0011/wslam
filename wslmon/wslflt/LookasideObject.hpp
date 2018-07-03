#ifndef _LOOKASIDE_OBJECT_
#define _LOOKASIDE_OBJECT_

#include "LookasideList.hpp"
#include "TypeTraits.hpp"

namespace WslFlt
{

    /*
    Lookaside<Clasa> ----|> Clasa
    */

    template <typename T>
    class LookasideObject
        : public T
    {
    public:
        template <typename ... Args>
        LookasideObject(
            _In_ Args&& ... Arguments
        ) : T{ Arguments ... }
        {}

        FORCEINLINE
        void*
        operator new(size_t)
        {
            return _List.Allocate();
        }

        void
        operator delete(
            _In_ _Pre_notnull_ void *Location
        )
        {
            _List.Free(Location);
        }
    private:
        static LookasideList<T> _List;
    };


    template <typename T>
    WslFlt::LookasideList<T> WslFlt::LookasideObject<T>::_List(
        WslFlt::TypeTraits<T>::NumberOfCachableElements
    );
}

#endif // _LOOKASIDE_OBJECT_