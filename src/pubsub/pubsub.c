/* Copyright (C) 2023 Jonas Strindlund - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the YYZ license, which unfortunately won't be
 * written for another century (2112).
 *
 * You should have received a copy of the YYZ license with
 * this file. If not, please write to: jonas.strindlund@gmail.com
 * or visit : ByTor.yyz
 *
 */
#include <stdio.h>
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <pico/time.h>

#include "pubsub.h"

//typedef struct pubsub pubsub;
typedef pubsub_err_t (*__subscribe)(void*, callback_t, int*);
typedef pubsub_err_t (*__unsubscribe)(void*, int);
typedef pubsub_err_t (*__runCallbacks)(void*, subscriptionEvent_t);

#define CALLBACK_MAX 10
static pubsub_err_t _unsubscribe(void* p, int subscriptionID);
static pubsub_err_t _subscribe(void* p, callback_t callback, int* subscriptionID);
static pubsub_err_t _runCallbacks(void* p, subscriptionEvent_t evn);
struct pubsub
{
   pubsub* self;
   int index;
   callback_t callbacks[CALLBACK_MAX];

   __subscribe subscribe;
   __unsubscribe unsubscribe;
   __runCallbacks runCallbacks;
};

/** \brief public subscribe method
 *
 * \param p void*
 * \param callback callback_t
 * \param subscriptionID int*
 * \return pubsub_err_t
 *
 */
pubsub_err_t pubsub_subscribe(void* p, callback_t callback, int* subscriptionID)
{
   pubsub_err_t err = PUBSUB_SUB_ERROR;
   if (p != NULL)
      err = ((pubsub*)p)->subscribe(p, callback, subscriptionID);
   return err;
}

/** \brief public unsubscribe method
 *
 * \param p void*
 * \param subscriptionID int
 * \return pubsub_err_t
 *
 */
pubsub_err_t pubsub_unsubscribe(void* p, int subscriptionID)
{
   pubsub_err_t err = PUBSUB_SUB_ERROR;
   if (p != NULL)
      err = ((pubsub*)p)->unsubscribe(p, subscriptionID);
   return err;
}

/** \brief public pubsub_runCallbacks
 *
 * \param p void*
 * \param evn subscriptionEvent_t
 * \return pubsub_err_t
 *
 */
pubsub_err_t pubsub_runCallbacks(void* p, subscriptionEvent_t evn)
{
   pubsub_err_t err = PUBSUB_SUB_ERROR;
   if (p != NULL)
      err = ((pubsub*)p)->runCallbacks(p, evn);
   return err;
}

/** \brief public init function
 *
 * \return pubsub_err_t
 *
 */
pubsub_err_t pubsub_init()
{
   pubsub_err_t err = PUBSUB_ERROR_NONE;
   return err;
}

/** \brief public create method
 *
 * \param void
 * \return pubsub*
 *
 */
pubsub* pubsub_create(void)
{
   pubsub* p = (pubsub*)malloc(sizeof(*p));
   if (p != NULL)
   {
      p->self = p;
      p->index = 0;

      p->subscribe = &_subscribe;
      p->unsubscribe = &_unsubscribe;
      p->runCallbacks = &_runCallbacks;

      memset(p->callbacks, 0, CALLBACK_MAX*sizeof(p->callbacks[0]));
   }
   return p;
}

/** \brief public destroy method
 *
 * \param p void*
 * \return void
 *
 */
void pubsub_destroy(void* p)
{
   if ((pubsub*)p != NULL)
   {
      ((pubsub*)p)->self = NULL;
      free((pubsub*)p);
      p = NULL;
   }
}

/** \brief internal unsubscribe method
 *
 * \param p void*
 * \param subscriptionID int
 * \return pubsub_err_t
 *
 */
pubsub_err_t _unsubscribe(void* p, int subscriptionID)
{
   pubsub_err_t err = PUBSUB_SUB_ERROR;

   if(((pubsub*)p)->self != NULL)
   {
      if (subscriptionID < CALLBACK_MAX)
      {
         if (((pubsub*)p)->self->callbacks[subscriptionID] != NULL)
         {
            ((pubsub*)p)->self->callbacks[subscriptionID] = NULL;
            ((pubsub*)p)->self->index = subscriptionID;           // fill the gap next time but check for NULLs
            err = PUBSUB_OK;
         }
      }
   }
   return err;
}

/** \brief internal subscribe method
 *
 * \param p void*
 * \param callback callback_t
 * \param subscriptionID int*
 * \return pubsub_err_t
 *
 */
pubsub_err_t _subscribe(void* p, callback_t callback, int* subscriptionID)
{
   pubsub_err_t err = PUBSUB_SUB_ERROR;
   if(((pubsub*)p)->self != NULL)
   {
//      printf("self %p , %d\n", ((pubsub*)p)->self, *subscriptionID);
      if (((pubsub*)p)->self->index < CALLBACK_MAX)
      {
         *subscriptionID = ((pubsub*)p)->self->index;
         ((pubsub*)p)->self->callbacks[((pubsub*)p)->self->index] = callback;
         do
         {
            ((pubsub*)p)->self->index++;
         } while (((pubsub*)p)->self->callbacks[((pubsub*)p)->self->index] != NULL);

         err = PUBSUB_OK;
      }
   }
   return err;
}

/** \brief internal _runCallbacks
 *
 * \param p void*
 * \param evn subscriptionEvent_t
 * \return pubsub_err_t
 *
 */
pubsub_err_t _runCallbacks(void* p, subscriptionEvent_t evn)
{
   pubsub_err_t err = PUBSUB_SUB_ERROR;
   if(((pubsub*)p)->self != NULL)
   {
//      printf("self %p , %s %d\n", ((pubsub*)p)->self, evn.eventHandler, evn.eventID);
      int n = ((pubsub*)p)->index;
      while (--n < (size_t)-1)
         ((pubsub*)p)->callbacks[n](evn);
   }
   return err;
}

