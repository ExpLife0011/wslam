#include <fltkernel.h>

int __cdecl
_purecall(void) {
    NT_ASSERTMSG("Pure virtual function call", false);
    return 0;
}