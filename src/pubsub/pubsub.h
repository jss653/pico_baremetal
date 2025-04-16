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
   PUBSUB_SUB_ERROR,
   PUBSUB_LAST_ERROR
} pubsub_err_t;

/** \brief subscriptionEvent_t contains information about
 *         eventHandler and eventID
 *
 * \param eventHandler
 * \param eventID
 *
 */
typedef struct
{
   const char* eventHandler;     /**< name of subscription provider */
   int eventID;                  /**< event ID. Incremented per event */
}subscriptionEvent_t;

typedef void (*callback_t)(subscriptionEvent_t);
typedef struct pubsub pubsub;

pubsub_err_t pubsub_init();

pubsub* pubsub_create(void);
void pubsub_destroy(void* p);

pubsub_err_t pubsub_subscribe(void*, callback_t, int*);
pubsub_err_t pubsub_unsubscribe(void*, int);

pubsub_err_t pubsub_runCallbacks(void*, subscriptionEvent_t evn);

#endif // __PUBSUB_H__
