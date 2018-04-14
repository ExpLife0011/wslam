#ifndef _UWPCOMM_H_
#define _UWPCOMM_H_

#ifdef UWPCOMM_EXPORTS
#define UWPCOMM_API __declspec(dllexport)
#else
#define UWPCOMM_API __declspec(dllimport)
#endif // UWPCOMM_EXPORTS

#define PACKAGE_FAMILY_NAME L"WslAntiMalware_pt6twq797941g"
#define APPSERVICE_NAME L"com.wsl.antimalware.notifications"
#define TOKEN   L"WSLAMSS"

#endif // _UWPCOMM_H_
