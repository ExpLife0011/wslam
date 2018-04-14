#ifndef _APP_COMMUNICATION_HPP_
#define _APP_COMMUNICATION_HPP_

#include <string>
#include "uwpcomm.h"

namespace UwpComm
{
    class AppServiceConnection
    {
    public:
        UWPCOMM_API
        AppServiceConnection() {}

        UWPCOMM_API
        void
        NotifyDetection(
            const std::wstring& ImageFileName
        );

        UWPCOMM_API
        ~AppServiceConnection() {}
    };

}

#endif // _APP_COMMUNICATION_HPP_
