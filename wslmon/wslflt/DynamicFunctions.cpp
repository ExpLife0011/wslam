#include "DynamicFunctions.hpp"
#include "wsl_status.hpp"


UNICODE_STRING gZwQueryInformationProcessFuncName = RTL_CONSTANT_STRING(L"ZwQueryInformationProcess");
EXTERN_C 
typedef
NTSTATUS
(NTAPI *PFUNC_ZwQueryInformationProcess) (
    _In_        HANDLE              ProcessHandle,
    _In_        PROCESSINFOCLASS    ProcessInformationClass,
    _Out_       PVOID               ProcessInformation,
    _In_        ULONG               ProcessInformationLength,
    _Out_opt_   PULONG              ReturnLength
);
static PFUNC_ZwQueryInformationProcess gZwQueryInformationProcessFunc = nullptr;

void
WslFlt::DynamicFunctions::Initialize()
{
    gZwQueryInformationProcessFunc = (PFUNC_ZwQueryInformationProcess)DynamicFunctions::GetSystemFunction(&gZwQueryInformationProcessFuncName);
}

_Use_decl_annotations_
NTSTATUS
WslFlt::DynamicFunctions::ZwQueryProcessInformation(
    _In_        HANDLE              ProcessHandle,
    _In_        PROCESSINFOCLASS    ProcessInformationClass,
    _Out_       PVOID               ProcessInformation,
    _In_        ULONG               ProcessInformationLength,
    _Out_opt_   PULONG              ReturnLength
)
{
    if (!gZwQueryInformationProcessFunc)
    {
        return WSL_DYNAMIC_FUNCTION_NOT_INITIALIZED;
    }

    return gZwQueryInformationProcessFunc(
        ProcessHandle,
        ProcessInformationClass,
        ProcessInformation,
        ProcessInformationLength,
        ReturnLength
    );
}