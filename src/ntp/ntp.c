#include <stdio.h>
#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/dns.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"


#include "ntp.h"

typedef struct NTP_T_
{
   ip_addr_t ntp_server_address;
   bool dns_request_sent;
   struct udp_pcb* ntp_pcb;
   absolute_time_t ntp_test_time;
   alarm_id_t ntp_resend_alarm;
} NTP_T;



/** \brief ntp_init
 *
 * \return ntp_err_t
 *
 */
ntp_err_t ntp_init()
{
   ntp_err_t err = NTP_ERROR_NONE;
   return err;
}
