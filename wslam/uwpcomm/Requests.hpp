#ifndef _REQUESTS_HPP_
#define _REQUESTS_HPP_


#define REQUEST_FIELD_STATUS            L"s"
#define REQUEST_FIELD_REQUEST_ID        L"ri"
#define REQUEST_FIELD_TOKEN             L"t"
#define REQUEST_FIELD_IMAGE_FILE_NAME   L"ifn"

#define COMM_SUCCESS(status) ((long)(status)>=0)

namespace UwpComm
{
    enum class RequestIds : int
    {
        HandShake,
        MonitoringStarted,
        MonitoringStopped,
        Detection
    };
}
#endif // _REQUESTS_HPP_
