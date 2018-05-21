#include "CppUnitTest.h"
#include "TestHeader.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace wslcoretests
{		
    TEST_CLASS(UnitTest1)
    {
    public:
        
        TEST_METHOD(TestMethod1)
        {
            WslCore::TS a{ 2 };
        }

    };
}