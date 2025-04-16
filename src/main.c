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

 /** \file main.c
 *
 *        The main purpose (pun intended) of this file is to provide an entry point for
 *        the smartheatpump application. The application is driven by a system timer, systickFunction
 *        and a state machine with state_e states.
 *
 *
 * \ref state_e

 * \ref wifi
 * \ref timer
 * \ref pubsub
 *
 */

#include <stdio.h>
//#include <unistd.h>
#include <errno.h>
//#include <string.h>
//#include <getopt.h>
//#include <sys/types.h>
//#include <libgen.h>
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include <pico/time.h>
#include "hardware/watchdog.h"

//#include "lwip/apps/httpd.h"
//#include "lwipopts.h"

#include "wifi/wifi.h"
#include "timer/timer.h"
#include "errorhandler/errorhandler.h"
#include "config.h"
#include "pubsub/pubsub.h"

struct state;
typedef void (*state_func_t)(struct state*);    /**< function pointer template for machine state handling */

typedef enum {STATE_DEFAULT, STATE_INIT, STATE_RUN, STATE_RESTART} state_e;      /**< machine states */

/** \brief class holding attributes for the state machine
 */
typedef struct state
{
    state_e currentState;     /**< Current state when entering the state function handler */

    state_func_t function;    /**< Callback to function, handling different state */

    int swStatus;             /**< Bit field indicating status */
    int hwStatus;             /**< Bit field indicating status */
    int runStatus;            /**< Bit field indicating status */
} state_t;

void state_init(state_t*);    /**< Function prototype for state STATE_INIT */
void state_run(state_t*);     /**< Function prototype for state STATE_RUN */
void state_restart(state_t*); /**< Function prototype for state STATE_RESTART */

void execute_state(state_t* n);  /**< Function prototype for state handler */

int swInit(state_t*);
int hwInit(state_t*);
// Initialize state machine
state_t state = {STATE_DEFAULT, state_init, 0,0,0}; //We're about to enter state init

/** \brief Callback function for systick
 *
 * \param event event_t
 * \return void
 *
 */
void timerEventHandler(subscriptionEvent_t event)
{
   watchdog_update();
   cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,(cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN) ^ 1));
//   printf("%s, %d\n",event.evHandler, event.type);
   execute_state(&state);
}

/** \brief main, entrypoint for the smarthp application
 *
 * \see timer_init
 * \see wifi_init
 * \see sensor_init
 * \see server_init
 *
 *
 * \param argc int
 * \param argv[] char*
 * \return int
 *
 */
int main(int argc, char* argv[])
{
   int subscriptionID = 0;

   // Init SDK and hw. This HAS to be done before anything else, hence the placement in main
   assert(stdio_init_all() == true);
   // Init SDK and hw
   assert(cyw43_arch_init() == SMARTHP_NO_ERROR);

// TODO (jss#5#03/17/25): Move this to SW_INIT once state machine is in place

   // Init all services available for this app
   // init functions should never fail unless there is secondary failure.
   // Assert to expect the unexpected
   assert(pubsub_init() == PUBSUB_ERROR_NONE);

   assert(timer_init() == TIMER_ERROR_NONE);
   timer_subscribe(timer_1000mS, &timerEventHandler, &subscriptionID);
 // TODO (jss#1#03/17/25): Move this to SW_INIT once state machine is in place
   assert(wifi_init() == WIFI_ERROR_NONE);
   wifi_connect();

   while(1);

   exit(EXIT_SUCCESS);
}

// ==========================================================================================================
/** \brief  Execution handler for state machine
 *
 *          The idea here is to use currentState
 *          to do preps before handling the actual
 *          state by the specific callback handler.
 *
 * \param state state_t*
 * \return void
 *
 */
void execute_state(state_t* state)
{
   switch (state->currentState)    /**< Well, at this point this is actually the current (or previous) state */
   {
      case STATE_INIT:
         printf("init\n");
         break;
      case STATE_RUN:
         printf("run\n");
         break;
      case STATE_RESTART:
   //           cyw43_deinit()
         printf("restart\n");
         break;
      default:
         break;
   }
   state->function(state);
   //        current_state = error_handler(result, current_state);
};

/** \brief  Handler for STATE_INIT
 *
 *          This handler is responsible for initializing both software and hardware
 *          so that we can transition to the next state STATE_RUN
 *
 * \param state state_t*
 * \return void
 *
 */
void state_init(state_t* state)
{
    state->currentState = STATE_INIT;

    swInit(state);
    hwInit(state);

    // go next state
    state->function = state_run;
}

/** \brief  Handler for STATE_RUN
 *
 *          This handler is responsible for all the magic
 *
 * \param state state_t*
 * \return void
 *
 */
void state_run(state_t* state)
{
    state->currentState = STATE_RUN;
}

/** \brief  Handler for STATE_RESTART
 *
 *          This handler is responsible for re-initializing both software and hardware
 *          so that we can transition to the next state STATE_INIT
 *
 * \param state state_t*
 * \return void
 *
 */
void state_restart(state_t* state)
{
    state->currentState = STATE_RESTART;
    // check if watchdog tripped
//    if (watchdog_enable_caused_reboot())
//        printf("Rebooted by Watchdog!\n");

    // go to next state
    state->function = state_init;
}

/** \brief swInit is responsible for software init
 *
 * \param state state_t*
 * \return int
 *
 */
int swInit(state_t* state)
{
   // Enable the watchdog, requiring the watchdog to be updated every 1500ms or the chip will reboot
   // second arg is pause on debug which means the watchdog will pause when stepping through code
   // NB: A negative period means "start to start" - i.e. regardless of how long the callback takes to run
   watchdog_enable(-1500, 1);

   return 0;
}

/** \brief swInit is responsible for hardware init
 *
 * \param state state_t*
 * \return int
 *
 */
int hwInit(state_t*)
{
    return 0;
}

/** \brief Visible hook to move int STATE_RESTART
 *
 * \return void
 *
 */
void restart()
{
   state_t state = {STATE_RESTART, state_restart, 0,0,0}; //We're about to enter state restart
   execute_state(&state);
}

