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
        AppServiceConnection()
        {
            _appServiceConnection = new UwpComm::AppServiceConnection();
        }

        void
        NotifyDetection(String^ ImageFileName)
        {
            _appServiceConnection->NotifyDetection(msclr::interop::marshal_as<std::wstring>(ImageFileName));
        }

        ~AppServiceConnection()
        {
            if (_appServiceConnection)
            {
                delete _appServiceConnection;
            }
        }
    private:
        UwpComm::AppServiceConnection * _appServiceConnection;
    };
}

#endif //_APP_COMMUNICATION_WRAPPER_HPP_
