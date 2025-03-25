#ifndef __PUBSUB_H__
#define __PUBSUB_H__
#include "../errorhandler/errorcodes.h"
#include "../config.h"

typedef enum
{
   PUBSUB_OK = SMARTHP_OK,
   PUBSUB_ERROR_NONE = SMARTHP_NO_ERROR,
   PUBSUB_TIMEOUT = PUBSUB_ERR_BASE,
   PUBSUB_NO_DATA,
   PUBSUB_LAST_ERROR
} pubsub_err_t;

pubsub_err_t pubsub_init();

#endif // __PUBSUB_H__
