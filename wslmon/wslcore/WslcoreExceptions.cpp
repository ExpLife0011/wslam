#define WIN32_NO_STATUS
#include <Windows.h>
#undef WIN32_NO_STATUS
#include <locale>
#include <codecvt>
#include "WslcoreExceptions.hpp"


_Use_decl_annotations_
WslCore::WslCoreException::WslCoreException(
    _In_ std::wstring Message,
    _In_ long ErrorCode
) : std::exception{}, _ErrorCode{ ErrorCode }
{
    std::wstringstream stream;
    stream << Message;
    if (!IS_WSL_STATUS(ErrorCode))
    {
        FormatErrorMessage(ErrorCode, stream);
    }
    else
    {
        stream << "Error: " << std::hex << ErrorCode;
    }

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string convertedString = converter.to_bytes(stream.str());

    _ErrorMessage = (char*) new char[convertedString.size() + 1];
    CopyMemory((void*)_ErrorMessage, (void*)convertedString.c_str(), convertedString.size() + 1);
}


const char*
WslCore::WslCoreException::what() const
{
    return _ErrorMessage;
}

WslCore::WslCoreException::~WslCoreException()
{
    delete[] _ErrorMessage;
}

_Use_decl_annotations_
void
WslCore::WslCoreException::FormatErrorMessage(
    _In_ long ErrorCode,
    _In_ std::wstringstream & stream
)
{
    if (ErrorCode)
    {
        wchar_t* lpMsgBuf = nullptr;;
        DWORD bufLen = ::FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            ErrorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0,
            NULL
        );

        if (lpMsgBuf)
        {
            LPWSTR lpMsgStr = (LPWSTR   )lpMsgBuf;
            std::wstring result(lpMsgStr, lpMsgStr+bufLen);
            LocalFree(lpMsgBuf);
            stream << "Error: " << std::hex << ErrorCode << ". " << result;
        }
    }
}

