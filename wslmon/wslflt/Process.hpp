#ifndef _PROCESS_HPP_
#define _PROCESS_HPP_

#include <fltKernel.h>
#include "TypeTraits.hpp"

namespace WslFlt
{
    class Process
    {
    public:
        Process(
            _In_ HANDLE ProcessId,
            _In_ HANDLE Token,
            _In_ bool IsElevated
        ) : _ProcessId{ ProcessId }, _Token{ Token }, _IsElevated{ IsElevated }
        {}

        Process()
            : _ProcessId{ 0 }, _Token{ nullptr }
        {}
        
        virtual ~Process();

        HANDLE
        ProcessId() const
        {
            return _ProcessId;
        }

        HANDLE
        Token()
        {
            return _Token;
        }

        bool
        IsElevated() const
        {
            return _IsElevated;
        }

        void
        IsElevated(bool NewElevation)
        {
            _IsElevated = NewElevation;
        }

        LIST_ENTRY ProcessCollectorEntry;

    private:
        HANDLE _ProcessId;
        HANDLE _Token;
        bool _IsElevated;
    };

    template<>
    struct TypeTraits<Process>
    {
        static constexpr ULONG Tag = WSLFLT_TAG_PROCESS;
        static constexpr POOL_TYPE PoolType = NonPagedPool;
        static constexpr LONG NumberOfCachableElements = DEFAULT_NUMBER_OF_CACHABLE_ELEMENTS;
    };
}

#endif // _PROCESS_HPP_
