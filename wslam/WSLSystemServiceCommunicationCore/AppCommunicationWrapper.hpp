#ifndef _APP_COMMUNICATION_WRAPPER_HPP_
#define _APP_COMMUNICATION_WRAPPER_HPP_

#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

#include "AppCommunication.hpp"

using namespace System;
using namespace System::Configuration;
using namespace System::Runtime::InteropServices;

namespace WSLSystemServiceCommunicationCore
{
    public ref class AppServiceConnection
    {
    public:
        AppServiceConnection(
            String^ Token
        )
        {
            _AppServiceConnection = new UwpComm::AppServiceConnection{ msclr::interop::marshal_as<std::wstring>(Token) };
        }

        void
        NotifyDetection(String^ ImageFileName)
        {
            _AppServiceConnection->NotifyDetection(msclr::interop::marshal_as<std::wstring>(ImageFileName));
        }

        ~AppServiceConnection()
        {
            delete _AppServiceConnection;
        }
    private:
        UwpComm::AppServiceConnection* _AppServiceConnection;
    };
}

#endif //_APP_COMMUNICATION_WRAPPER_HPP_
