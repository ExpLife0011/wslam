#ifndef _WSLCORE_EXCEPTIONS_HPP_
#define _WSLCORE_EXCEPTIONS_HPP_

#include <sal.h>
#include <exception>
#include <string>
#include <sstream>
#include "..\wslcommon\wsl_status.hpp"

namespace WslCore
{
    class WslCoreException
        : public std::exception
    {
    public:
        WslCoreException(
            _In_ std::wstring Message,
            _In_ long ErrorCode
        );

        WslCoreException(
            _In_ std::wstring Message,
            _In_ unsigned long ErrorCode
        ) : WslCoreException{ Message, (long)ErrorCode }
        {}

        const char*
        what() const override;

        virtual
        ~WslCoreException();

    protected:
        void
        FormatErrorMessage(
            _In_ long ErrorCode,
            _In_ std::wstringstream& stream
        );

    private:
        char* _ErrorMessage;
        long _ErrorCode;
    };
    
    class ExWin32Exception
        : public WslCoreException
    {
    public:
        ExWin32Exception(
            _In_ std::wstring Message,
            _In_ long ErrorCode
        ) : WslCoreException{ Message, ErrorCode }
        {}

        ExWin32Exception(
            _In_ std::wstring Message,
            _In_ unsigned long ErrorCode
        ) : WslCoreException{ Message, ErrorCode }
        {}
    };

    class ExCommunicationException
        : public WslCoreException
    {
    public:
        ExCommunicationException(
            _In_ const wchar_t* Message,
            _In_ long ErrorCode
        ) : WslCoreException{ Message, ErrorCode }
        {}

        ExCommunicationException(
            _In_ const wchar_t* Message,
            _In_ unsigned long ErrorCode
        ) : WslCoreException{ Message, ErrorCode }
        {}
    };
}

#endif // _WSLCORE_EXCEPTIONS_HPP_
