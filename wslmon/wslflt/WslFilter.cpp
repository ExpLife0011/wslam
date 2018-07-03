#include "WslFilter.hpp"
//#include "Global.hpp"
#include "FilterRegistration.hpp"
#include "staticclassessupport.h"
#include "ProcessFilter.hpp"
#include "wslflt_trace.h"
#include "WslFilter.cpp.tmh"
#include "DynamicFunctions.hpp"


__declspec(code_seg("PAGE")) 
_Use_decl_annotations_
WSLSTATUS
WslFlt::WslFilter::Initialize(
    PDRIVER_OBJECT DriverObject,
    PUNICODE_STRING RegistryPath
)
{
    WSLSTATUS status = STATUS_UNSUCCESSFUL;
    bool registeredFilter = false;
    
    UNREFERENCED_PARAMETER(RegistryPath);

    WslFlt::DynamicFunctions::Initialize();
    
    status = ::FltRegisterFilter(
        DriverObject,
        &WslFlt::FilterRegistration,
        &_Filter
    );
    FLT_ASSERT(WSL_SUCCESS(status));
    if (!WSL_SUCCESS(status))
    {
        goto CleanUp;
    }
    registeredFilter = true;


    status = ProcessFilter::Instance().StartProcessFiltering();
    if (!WSL_SUCCESS(status))
    {
        goto CleanUp;
    }

    status = ::FltStartFiltering(_Filter);
    if (!WSL_SUCCESS(status))
    {
        goto CleanUp;
    }

    _DriverObject = DriverObject;

    status = STATUS_SUCCESS;

CleanUp:
    if (!WSL_SUCCESS(status))
    {
        if (registeredFilter)
        {
            ::FltUnregisterFilter(_Filter);
        }
    }

    return status;
}

#include "SharedPointer.hpp"
class C
{

};

__declspec(code_seg("PAGE"))
_Use_decl_annotations_
NTSTATUS
WslFlt::WslFilter::WslUnload(
    _In_ FLT_FILTER_UNLOAD_FLAGS Flags
)
{
    UNREFERENCED_PARAMETER(Flags);
    ProcessFilter::Instance().StopProcessFiltering();
    CppExit();
    WPP_CLEANUP(WslFilter::Instance()._DriverObject);

    return STATUS_SUCCESS;
}


__declspec(code_seg("PAGE")) 
_Use_decl_annotations_
NTSTATUS
FLTAPI
WslFlt::WslFilter::WslInstanceSetup(
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_ FLT_INSTANCE_SETUP_FLAGS Flags,
    _In_ DEVICE_TYPE VolumeDeviceType,
    _In_ FLT_FILESYSTEM_TYPE VolumeFilesystemType
)
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(Flags);
    UNREFERENCED_PARAMETER(VolumeDeviceType);
    UNREFERENCED_PARAMETER(VolumeFilesystemType);

    return STATUS_SUCCESS;
}


__declspec(code_seg("PAGE")) 
_Use_decl_annotations_
NTSTATUS
FLTAPI
WslFlt::WslFilter::WslInstanceQueryTeardown(
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
)
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(Flags);

    return STATUS_SUCCESS;
}


__declspec(code_seg("PAGE")) 
_Use_decl_annotations_
VOID
FLTAPI
WslFlt::WslFilter::WslInstanceTeardownStart(
    _In_ PCFLT_RELATED_OBJECTS FltObjects, 
    _In_ FLT_INSTANCE_TEARDOWN_FLAGS Flags
)
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(Flags);
}


__declspec(code_seg("PAGE")) 
_Use_decl_annotations_
VOID
FLTAPI
WslFlt::WslFilter::WslInstanceTeardownComplete(
    _In_ PCFLT_RELATED_OBJECTS FltObjects, 
    _In_ FLT_INSTANCE_TEARDOWN_FLAGS Flags
)
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(Flags);
}
