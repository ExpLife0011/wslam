#include <fltKernel.h>
#include <suppress.h>
#include "staticclassessupport.h"
#include "wslflt_trace.h"
#include "wslflt.cpp.tmh"

#pragma prefast(disable:__WARNING_ENCODE_MEMBER_FUNCTION_POINTER, "Not valid for kernel mode drivers")

EXTERN_C
DRIVER_INITIALIZE DriverEntry;

EXTERN_C
NTSTATUS
DriverEntry (
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
);

#pragma alloc_text(INIT, DriverEntry)


EXTERN_C
NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
)
/*++

Routine Description:

This is the initialization routine for this miniFilter driver.  This
registers with FltMgr and initializes all global data structures.

Arguments:

DriverObject - Pointer to driver object created by the system to
represent this driver.

RegistryPath - Unicode string identifying where the parameters for this
driver are located in the registry.

Return Value:

Returns STATUS_SUCCESS.

--*/
{
    PAGED_CODE();
    WPP_INIT_TRACING(DriverObject, RegistryPath);

    CppInitExitDescriptorsList();
    CppCallStaticConstructors(__crtXca, __crtXcz);
    if (!CppWereAllDestructorsRegistered())
    {
        return STATUS_UNSUCCESSFUL;
    }

    WslLogError("CPP support initialization done.");

    //
    // Actual filter registration is done here.
    //
    //return WslFlt::WslFilter::Instance().Initialize(DriverObject, RegistryPath);
    return STATUS_SUCCESS;
}
