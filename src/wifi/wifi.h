#ifndef __WIFI_H__
#define __WIFI_H__
#include "../errorhandler/errorcodes.h"
#include "../config.h"

typedef enum
{
   WIFI_OK = SMARTHP_OK,
   WIFI_ERROR_NONE = SMARTHP_NO_ERROR,
   WIFI_TIMEOUT = WIFI_ERR_BASE,
   WIFI_NO_DATA,
   WIFI_LAST_ERROR
} wifi_err_t;


wifi_err_t wifi_init();
wifi_err_t wifi_connect();

#endif //__WIFI_H__
