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
#include "../pubsub/pubsub.h"

#include "timer.h"

//================ variables ======================
static int64_t timerTick1s = 0;
static int64_t timerTick10s = 0;

/** \brief repeating_timer timer1s
 */
static struct repeating_timer timer1s;

/** \brief repeating_timer timer10s
 */
static struct repeating_timer timer10s;

//================ variables end ==================

//============ forward declarations ===============
/** \brief timer1000mSHandler handler for the 1000mS timer
 *
 * \param struct repeating_timer*
 * \return bool
 *
 */
static bool timer1000mSHandler (struct repeating_timer* t);  /**< Forward declaration of 1000mS  handler */

/** \brief timer1000mSHandler handler for the 10000mS timer
 *
 * \param struct repeating_timer*
 * \return bool
 *
 */
static bool timer10000mSHandler (struct repeating_timer* t);  /**< Forward declaration of 10000mS  handler */
//========== forward declarations end =============

//============= interface functions ===============
//=========== interface functions end =============

static pubsub* timer1s_p = NULL;
static pubsub* timer10s_p = NULL;

/** \brief Timer initilization
 *
 *         Init functions should never fail unless it is a secondary/unexpected fail.
 *         You should assert(<module>_init() == <module>_NO_ERROR)
 * \return timer_err_t
 *
 */
timer_err_t timer_init()
{
   timer_err_t err = TIMER_NO_DATA;

   // Negative delay so means we will call repeating_timer_callback, and call it again
   // delay ms later regardless of how long the callback took to execute
   // set up pubsub and enable subscription service from timer
   if ((timer1s_p = pubsub_create()) != NULL)
      if ((timer10s_p = pubsub_create()) != NULL)
      {
         err = add_repeating_timer_ms(-1000, timer1000mSHandler, NULL, &timer1s) ? TIMER_ERROR_NONE : TIMER_TIMEOUT;
         if (err == TIMER_ERROR_NONE)
            err = add_repeating_timer_ms(-10000, timer10000mSHandler, NULL, &timer10s) ? TIMER_ERROR_NONE : TIMER_TIMEOUT;
      }

   return err;
}

// ============== timer sub implementation =============================
#define TIMER_CALLBACK_MAX 10

static const char* timerEvents_s[] = {
   #define X(N) #N,
     timerEvents
   #undef X
};


static callback_t callbacks1s[TIMER_CALLBACK_MAX];
static callback_t callbacks10s[TIMER_CALLBACK_MAX];

/** \brief
 *
 * \param callback callback_t
 * \return timer_err_t
 *
 */
timer_err_t timer_subscribe(timerEvents_t eventType, callback_t callback, int* subscriptionID)
{
   timer_err_t err = TIMER_SUB_ERROR;

   switch (eventType)
   {
      case timer_1000mS:
      {
         if (timer1s_p != NULL)
            err = pubsub_subscribe(timer1s_p, callback, subscriptionID);
      }
      break;

      case timer_10000mS:
      {
         if (timer10s_p != NULL)
            err = pubsub_subscribe(timer10s_p, callback, subscriptionID);
      }
      break;

      default:
      break;
   }

   return err;
}

/** \brief
 *
 * \param subscriptionID int
 * \return timer_err_t
 *
 */
timer_err_t timer_unsubscribe(timerEvents_t eventType, int subscriptionID)
{
   timer_err_t err = TIMER_SUB_ERROR;

   switch (eventType)
   {
      case timer_1000mS:
      {
         if (timer1s_p != NULL)
            err = pubsub_unsubscribe(timer1s_p, subscriptionID);
      }
      break;

      case timer_10000mS:
      {
         if (timer10s_p != NULL)
            err = pubsub_unsubscribe(timer10s_p, subscriptionID);
      }
      break;

      default:
      break;
   }

   return err;
}
// ============== timer sub implementation end ==========================

//=============== helper functions ================

/** \brief timer1000mSHandler
 *
 * \param struct repeating_timer* t
 * \return bool always true
 *
 */
bool timer1000mSHandler (struct repeating_timer* t)
{
   subscriptionEvent_t evn = {timerEvents_s[timer_1000mS], timerTick1s++};

   pubsub_err_t err = pubsub_runCallbacks(timer1s_p, evn) ? TIMER_ERROR_NONE : TIMER_NO_DATA;

   return true;
}

/** \brief timer10000mSHandler
 *
 * \param struct repeating_timer* t
 * \return bool always true
 *
 */
bool timer10000mSHandler (struct repeating_timer* t)
{
   subscriptionEvent_t evn = {timerEvents_s[timer_10000mS], timerTick10s++};

   pubsub_err_t err = pubsub_runCallbacks(timer10s_p, evn) ? TIMER_ERROR_NONE : TIMER_NO_DATA;

   return true;
}
//============ helper functions end ===============

//    add_repeating_timer_ms(-500, repeating_timer_callback, NULL, &timer);
//    sleep_ms(3000);
//    cancelled = cancel_repeating_timer(&timer);
//
//#include "pico/stdlib.h"
//#include "hardware/irq.h"
//#include "hardware/alarm.h"
//
//// Define callback functions for each timer
//bool timer1_callback(struct repeating_timer *t) {
//    printf("Timer 1 fired!\n");
//    return true;
//}
//
//bool timer2_callback(struct repeating_timer *t) {
//    printf("Timer 2 fired!\n");
//    return true;
//}
//
//// Define repeating_timer structures globally
//struct repeating_timer timer1;
//struct repeating_timer timer2;
//
//// Define alarm pools
//alarm_pool_t *pool1;
//alarm_pool_t *pool2;
//
//int main() {
//    stdio_init_all();
//
//    // Create alarm pools for each core
//    pool1 = alarm_pool_create(0, 1);
//    pool2 = alarm_pool_create(1, 1);
//
//    // Add repeating timers to different pools
//    alarm_pool_add_repeating_timer_ms(pool1, 500, timer1_callback, NULL, &timer1);
//    alarm_pool_add_repeating_timer_ms(pool2, 1000, timer2_callback, NULL, &timer2);
//
//    // Main loop to keep the program running
//    while (true) {
//        tight_loop_contents();
//    }
//}
//




