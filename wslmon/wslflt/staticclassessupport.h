#ifndef _STATIC_CLASSES_SUPPORT_H_
#define _STATIC_CLASSES_SUPPORT_H_

#include <fltKernel.h>
#include "crtsection.h"

bool
CppWereAllDestructorsRegistered();

void
CppCallStaticConstructors(
    _In_ PVFV *Head,
    _In_ PVFV *Tail
);

void
CppInitExitDescriptorsList();

void
__cdecl
CppExit();


#endif // _STATIC_CLASSES_SUPPORT_H_

