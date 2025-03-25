#ifndef __SERVER_H__
#define __SERVER_H__
#include "../errorhandler/errorcodes.h"
#include "../config.h"

typedef enum
{
   SERVER_OK = SMARTHP_OK,
   SERVER_ERROR_NONE = SMARTHP_NO_ERROR,
   SERVER_TIMEOUT = SERVER_ERR_BASE,
   SERVER_NO_DATA,
   SERVER_LAST_ERROR
} server_err_t;


server_err_t server_init();

#endif //__SERVER_H__
