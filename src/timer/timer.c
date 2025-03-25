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

#include "timer.h"

static int64_t timerTick = 0;

bool timer1000mSHandler (struct repeating_timer* t);  /**< Forward declaration of 1000mS  handler */

/** \brief repeating_timer timer
 */
struct repeating_timer timer;

/** \brief Timer initilization
 *
 *         Init functions should never fail unless it is a secondary/unexpected fail.
 *         You should assert(<module>_init() == <module>_NO_ERROR)
 * \return timer_err_t
 *
 */
timer_err_t timer_init()
{
   return add_repeating_timer_ms(1000, timer1000mSHandler, NULL, &timer) ? TIMER_ERROR_NONE : TIMER_TIMEOUT;
}

// ============== timer sub implementation =============================
#define TIMER_CALLBACK_MAX 10


static callback_t callbacks[TIMER_CALLBACK_MAX];
static size_t n = 0;

/** \brief
 *
 * \param callback callback_t
 * \return timer_err_t
 *
 */
timer_err_t register_callback(callback_t callback)
{
   timer_err_t err = TIMER_ERROR_NONE;

   if (n < TIMER_CALLBACK_MAX)
      callbacks[n++] = callback;
   else
      err = TIMER_SUB_ERROR;

   return err;
}

/** \brief run_callbacks loops through the list of subscribers
 *
 * \param void
 * \return void
 *
 */
void run_callbacks(void)
{
   event_t evn = {"timer1000mS", timerTick++};
   int m = n;
   while (--m < (size_t)-1)
      callbacks[m](evn);
}

// ============== timer sub implementation end ==========================











// ================================================================================
/** \brief timer1000mSHandler
 *
 * \param struct repeating_timer* t
 * \return bool always true
 *
 */
bool timer1000mSHandler (struct repeating_timer* t)
{
   run_callbacks();

   return true;
}
