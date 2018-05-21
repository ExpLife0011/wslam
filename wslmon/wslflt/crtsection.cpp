#include "crtsection.h"

#define DECLSPEC_ALLOCATE(x) __declspec(allocate(x))

#pragma section(".CRT$XCA", long, read)
#pragma section(".CRT$XCZ", long, read)

DECLSPEC_ALLOCATE(".CRT$XCA") PVFV __crtXca[] = { 0 };
DECLSPEC_ALLOCATE(".CRT$XCZ") PVFV __crtXcz[] = { 0 };

#pragma comment(linker, "/merge:.CRT=.rdata")