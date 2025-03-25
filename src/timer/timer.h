#ifndef __TIMER_H__
#define __TIMER_H__
#include "../errorhandler/errorcodes.h"
#include "../config.h"

typedef enum
{
   TIMER_OK = SMARTHP_OK,
   TIMER_ERROR_NONE = SMARTHP_NO_ERROR,
   TIMER_TIMEOUT = TIMER_ERR_BASE,
   TIMER_NO_DATA,
   TIMER_SUB_ERROR,
   TIMER_LAST_ERROR
} timer_err_t;

timer_err_t timer_init();

// TODO (jss#5#03/17/25): Move this to a pub/sub service in pubsub
typedef struct
{
   char* evHandler;
   int type;
}event_t;
typedef void (*callback_t)(event_t evt);
timer_err_t register_callback(callback_t callback);


#endif // __TIMER_H__
