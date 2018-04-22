#include "AppCommunication.hpp"
#include "Requests.hpp"

#include "winrt/Windows.Storage.h"
#include "winrt/Windows.Storage.Streams.h"
#include "winrt/Windows.ApplicationModel.AppService.h"
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.Foundation.h"

#include <iostream>

#pragma comment(lib, "windowsapp") 


struct AppSrv
{
    winrt::Windows::ApplicationModel::AppService::AppServiceConnection Connection;
};


#include <iostream>
#include <functional>


void UwpComm::AppServiceConnection::NotifyDetection(
    const std::wstring& ImageFileName
)
{
    winrt::Windows::ApplicationModel::AppService::AppServiceConnection appServiceConnection;
    winrt::Windows::Foundation::Collections::ValueSet detectionNotification;

    appServiceConnection.PackageFamilyName(PACKAGE_FAMILY_NAME);
    appServiceConnection.AppServiceName(APPSERVICE_NAME);
    auto openStatus = appServiceConnection.OpenAsync().get();
    if (openStatus != winrt::Windows::ApplicationModel::AppService::AppServiceConnectionStatus::Success)
    {
        std::cout << "connect error " << (int)openStatus << " " << ::GetLastError() << std::endl;
        return;
    }

    detectionNotification.Insert(REQUEST_FIELD_REQUEST_ID, winrt::Windows::Foundation::PropertyValue::CreateInt32(static_cast<int>(NotificationId::Detection)));
    detectionNotification.Insert(REQUEST_FIELD_IMAGE_FILE_NAME, winrt::Windows::Foundation::PropertyValue::CreateString(ImageFileName));
    auto response = appServiceConnection.SendMessageAsync(detectionNotification).get();
    if (response.Status() != winrt::Windows::ApplicationModel::AppService::AppServiceResponseStatus::Success)
    {
        std::cout << "error detection notification sned " << (int)response.Status() << std::endl;
        return;
    }
}
