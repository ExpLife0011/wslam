#ifndef _DYNAMIC_FUNCTIONS_HPP
#define _DYNAMIC_FUNCTIONS_HPP

#include <fltKernel.h>

namespace WslFlt
{
    class DynamicFunctions
    {
    public:
        static
        _Must_inspect_result_
        NTSTATUS
        ZwQueryProcessInformation(
            _In_        HANDLE              ProcessHandle,
            _In_        PROCESSINFOCLASS    ProcessInformationClass,
            _Out_       PVOID               ProcessInformation,
            _In_        ULONG               ProcessInformationLength,
            _Out_opt_   PULONG              ReturnLength
        );

        static 
        void
        Initialize();

    private:
        DynamicFunctions() = delete;
        
        FORCEINLINE
        static
        _Must_inspect_result_
        PVOID
        GetSystemFunction(
            _In_ PUNICODE_STRING Name
        )
        {
            return ::MmGetSystemRoutineAddress(Name);
        }
    };
}

#endif // _DYNAMIC_FUNCTIONS_HPP
