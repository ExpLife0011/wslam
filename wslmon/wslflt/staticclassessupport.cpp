#include <fltKernel.h>
#include "crtsection.h"

#define CPP_DESTRUCTOR_TAG 'DPC!'

typedef struct _CPP_EXIT_DESCRIPTOR
{
    LIST_ENTRY ListEntry;
    PVFV Destructor;
}CPP_EXIT_DESCRIPTOR, *PCPP_EXIT_DESCRIPTOR;

static LIST_ENTRY gExitDescriptors;
static bool gSomeAllocationFailed = false;


bool
CppWereAllDestructorsRegistered()
{
    return !gSomeAllocationFailed;
}


void
CppCallStaticConstructors(
    _In_ PVFV *Head,
    _In_ PVFV *Tail
)
{
    PVFV *pfnCurrent = Head;

    while (pfnCurrent < Tail) {
        if (*pfnCurrent) {
            (**pfnCurrent)();
        }
        ++pfnCurrent;
    }
}


void
CppInitExitDescriptorsList()
{
    InitializeListHead(&gExitDescriptors);
}


PVFV
CppRegisterDestructor(
    _In_ PVFV Destructor
)
{
    PCPP_EXIT_DESCRIPTOR pExitDescriptor = (PCPP_EXIT_DESCRIPTOR)ExAllocatePoolWithTag(
        NonPagedPoolNx,
        sizeof(CPP_EXIT_DESCRIPTOR),
        CPP_DESTRUCTOR_TAG
    );
    if (!pExitDescriptor) {
        gSomeAllocationFailed = true;
        return nullptr;
    }

    pExitDescriptor->Destructor = Destructor;
    InsertHeadList(&gExitDescriptors, &pExitDescriptor->ListEntry);

    return Destructor;
}


void
CppCallDestructors()
{
    PCPP_EXIT_DESCRIPTOR pExitDescriptor = nullptr;

    while (!IsListEmpty(&gExitDescriptors))
    {
        pExitDescriptor = (PCPP_EXIT_DESCRIPTOR)RemoveHeadList(&gExitDescriptors);

        __try
        {
            if (pExitDescriptor->Destructor)
            {
                pExitDescriptor->Destructor();
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
        }

        ExFreePoolWithTag(pExitDescriptor, CPP_DESTRUCTOR_TAG);
    }
}


void
__cdecl
CppExit() {
    CppCallDestructors();
}


int
__cdecl
atexit(
    _In_ PVFV Function
)
{
    return (!CppRegisterDestructor(Function)) ? -1 : 0;
}
