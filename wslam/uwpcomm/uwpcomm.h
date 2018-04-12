#ifndef _UWPCOMM_H_
#define _UWPCOMM_H_

#ifdef UWPCOMM_EXPORTS
#define UWPCOMM_API __declspec(dllexport)
#else
#define UWPCOMM_API __declspec(dllimport)
#endif // UWPCOMM_EXPORTS

#define PACKAGE_FAMILY_NAME L"a13c5e57-4d88-41bb-8c5b-d48228a13278_pt6twq797941g"
#define APPSERVICE_NAME L"com.amermeze.wslam.detectionlistener"
#define TOKEN   L"WSLAMSS"

#endif // _UWPCOMM_H_
