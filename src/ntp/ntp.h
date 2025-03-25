#ifndef __NTP_H__
#define __NTP_H__
#include "../errorhandler/errorcodes.h"
#include "../config.h"

typedef enum
{
   NTP_OK = SMARTHP_OK,
   NTP_ERROR_NONE = SMARTHP_NO_ERROR,
   NTP_TIMEOUT = NTP_ERR_BASE,
   NTP_NO_DATA,
   NTP_LAST_ERROR
} ntp_err_t;

ntp_err_t ntp_init();

#endif // __NTP_H__
