#ifndef _WSLCORE_H_
#define _WSLCORE_H_

#ifndef TEST_BUILD
#ifdef WSLCORE_EXPORTS
#define WSLCORE_API __declspec(dllexport)
#else
#define WSLCORE_API __declspec(dllimport)
#endif // WSLCORE_EXPORTS
#else
#define WSLCORE_API
#endif // TEST_BUILD

#endif // _WSLCORE_H_