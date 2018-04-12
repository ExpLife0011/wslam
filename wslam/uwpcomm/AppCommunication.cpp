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




//UwpComm::AppServiceConnection::(
//    std::wstring AppServiceName,
//    std::wstring PackageFamilyName
//)
//{
    //auto con = new AppSrv{};
    //std::wcout << AppServiceName << " " << PackageFamilyName << std::endl;
    //con->Connection.PackageFamilyName(PackageFamilyName);
    //con->Connection.AppServiceName(AppServiceName);
    //auto i = con->Connection.OpenAsync().get();
    //std::cout << (int)i << std::endl;
    //this->_Connection = con;
//}
////
//API
//int
//Communication::AppServiceCon::Send()
//{
//    auto con = (Con*)_Connection;
//    ValueSet vs;
//    vs.Insert(L"Request-Id", PropertyValue::CreateInt32(0));
//    vs.Insert(L"Token", PropertyValue::CreateString(L"WSLAMSS"));
//    auto response = con->Connection.SendMessageAsync(vs).get();
//    auto status = response.Message().Lookup(L"status").as<IReference<int>>().GetInt32();
//    return status;
//}
//
//void Communication::AppServiceCon::OnMessageReceived(cbptr Callback)
//{
//    auto con = (Con*)_Connection;
//
//    con->Connection.RequestReceived(
//        [this, Callback]
//    (Windows::ApplicationModel::AppService::AppServiceConnection AppServiceConnection, Windows::ApplicationModel::AppService::AppServiceRequestReceivedEventArgs Arguments)
//    {
//        auto reqid = Arguments.Request().Message().Lookup(L"status").as<IReference<int>>().GetInt32();
//        Callback(*this, reqid);
//    }
//    );
//
//}
//
//
//API
//Communication::AppServiceCon::~AppServiceCon()
//{
//    auto con = (Con*)_Connection;
//    con->Connection.Close();
//    delete _Connection;
//}

UwpComm::AppServiceConnection::AppServiceConnection(
    const std::wstring& Token
)
{
    auto appService = new AppSrv{};
    appService->Connection.PackageFamilyName(PACKAGE_FAMILY_NAME);
    appService->Connection.AppServiceName(APPSERVICE_NAME);
    auto connectStatus = appService->Connection.OpenAsync().get();

    if (connectStatus != winrt::Windows::ApplicationModel::AppService::AppServiceConnectionStatus::Success)
    {
        return;
    }

    winrt::Windows::Foundation::Collections::ValueSet registrationMessage;
    registrationMessage.Insert(REQUEST_FIELD_REQUEST_ID, winrt::Windows::Foundation::PropertyValue::CreateInt32(static_cast<int>(RequestIds::HandShake)));
    registrationMessage.Insert(REQUEST_FIELD_TOKEN, winrt::Windows::Foundation::PropertyValue::CreateString(Token));

    auto response = appService->Connection.SendMessageAsync(registrationMessage).get();
    if (response.Status() != winrt::Windows::ApplicationModel::AppService::AppServiceResponseStatus::Success
        || !COMM_SUCCESS(response.Message().Lookup(REQUEST_FIELD_STATUS).as<winrt::Windows::Foundation::IReference<int>>().GetInt32()))
    {
        return;
    }

    this->_AppService = appService;
}

void UwpComm::AppServiceConnection::NotifyDetection(
    const std::wstring& ImageFileName
)
{
    auto appService = reinterpret_cast<AppSrv*>(_AppService);
    winrt::Windows::Foundation::Collections::ValueSet detectionNotification;
    detectionNotification.Insert(REQUEST_FIELD_REQUEST_ID, winrt::Windows::Foundation::PropertyValue::CreateInt32(static_cast<int>(RequestIds::Detection)));
    detectionNotification.Insert(REQUEST_FIELD_IMAGE_FILE_NAME, winrt::Windows::Foundation::PropertyValue::CreateString(ImageFileName));
    auto response = appService->Connection.SendMessageAsync(detectionNotification).get();
    if (response.Status() != winrt::Windows::ApplicationModel::AppService::AppServiceResponseStatus::Success)
    {
        ;
    }
}

UwpComm::AppServiceConnection::~AppServiceConnection()
{
    if (_AppService)
    {
        auto appService = reinterpret_cast<AppSrv*>(_AppService);
        appService->Connection.Close();
        delete appService;
    }
}
