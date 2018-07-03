#include "Process.hpp"

WslFlt::Process::~Process()
{
    if (_Token)
    {
        ::ZwClose(_Token);
    }
}
