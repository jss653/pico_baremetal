#ifndef __SENSOR_H__
#define __SENSOR_H__
#include "../errorhandler/errorcodes.h"
#include "../config.h"

typedef enum
{
   SENSOR_OK = SMARTHP_OK,
   SENSOR_ERROR_NONE = SMARTHP_NO_ERROR,
   SENSOR_TIMEOUT = SENSOR_ERR_BASE,
   SENSOR_NO_DATA,
   SENSOR_LAST_ERROR
} sensor_err_t;


sensor_err_t sensor_init();

#endif //__SENSOR_H__
