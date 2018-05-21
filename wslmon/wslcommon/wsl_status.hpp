#ifndef _WSL_STATUS_HPP_
#define _WSL_STATUS_HPP_

#include <ntstatus.h>
#include <sal.h>

typedef _Return_type_success_(return >= 0) long NTSTATUS;
typedef NTSTATUS *PNTSTATUS;

typedef _Return_type_success_(return >= 0) NTSTATUS WSLSTATUS;

#define IS_WSL_STATUS(status)           (((status) & 0x0FFF0000) == 0x0FFF0000)

#define WSL_STATUS_SUCCESS_BITS         0x2FFF0000L
#define WSL_STATUS_INFORMATION_BITS     0x6FFF0000L
#define WSL_STATUS_WARNING_BITS         0xAFFF0000L
#define WSL_STATUS_ERROR_BITS            0xFFFF0000L

#define WSL_SUCCESS(Status)             (((WSLSTATUS)(Status)) >= 0)
#define WSL_INFORMATION(Status)         ((((unsigned long)(Status)) >> 30) == 1)
#define WSL_WARNING(Status)             ((((unsigned long)(Status)) >> 30) == 2)
#define WSL_ERROR(Status)               ((((unsigned long)(Status)) >> 30) == 3)


#define WSL_DYNAMIC_FUNCTION_NOT_INITIALIZED        ((WSLSTATUS)WSL_STATUS_ERROR_BITS|0x0001)

#endif // _WSL_STATUS_HPP_