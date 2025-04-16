#ifndef __TIMER_H__
#define __TIMER_H__

#include "../errorhandler/errorcodes.h"
#include "../config.h"
#include "../pubsub/pubsub.h"

typedef enum
{
   TIMER_OK = SMARTHP_OK,
   TIMER_ERROR_NONE = SMARTHP_NO_ERROR,
   TIMER_TIMEOUT = TIMER_ERR_BASE,
   TIMER_NO_DATA,
   TIMER_SUB_ERROR,
   TIMER_LAST_ERROR
} timer_err_t;

/** \brief timerEvents is a list fo supported timer events.
 *         List expands to enum timerEvents_t and char* [] timerEvents_s
 *
 */
#define timerEvents \
   X(timer_1000mS) \
   X(timer_10000mS)

typedef enum
{
   #define X(N) N,
   timerEvents
   #undef X
}timerEvents_t;


/** \brief timer_init Initializes all supported timers and enables subscription
 *
 * \return timer_err_t
 *
 */
timer_err_t timer_init();

/** \brief timer_subscribe
 *
 * \param callback callback_t
 * \param subscriptionID int* allocated subscription ID, needed for canceling your subscription
 * \return timer_err_t
 *
 */
timer_err_t timer_subscribe(timerEvents_t eventType, callback_t callback, int* subscriptionID);

/** \brief timer_unsubscribe
 *
 * \param subscriptionID int
 * \return timer_err_t
 *
 */
timer_err_t timer_unsubscribe(timerEvents_t eventType, int subscriptionID);


#endif // __TIMER_H__
