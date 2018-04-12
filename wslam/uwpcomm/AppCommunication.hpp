#ifndef _APP_COMMUNICATION_HPP_
#define _APP_COMMUNICATION_HPP_

#include <string>
#include "uwpcomm.h"

namespace UwpComm
{
    class AppServiceConnection;


    class AppServiceConnection
    {
    public:
        UWPCOMM_API
        AppServiceConnection(
            const std::wstring& Token
        );

        UWPCOMM_API
        void
        NotifyDetection(
            const std::wstring& ImageFileName
        );


        UWPCOMM_API
        ~AppServiceConnection();

    private:
        void* _AppService;
    };

}

#endif // _APP_COMMUNICATION_HPP_
