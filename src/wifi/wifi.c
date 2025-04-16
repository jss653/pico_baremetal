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
//https://www.i-programmer.info/programming/hardware/16220-master-the-pico-wifi-simplest-http-client.html?start=3
//https://github.com/raspberrypi/pico-examples/blob/master/pico_w/wifi/iperf/picow_iperf.c

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <libgen.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

//#include "../lwipopts.h"
//
//#include "lwip/apps/httpd.h"

#include "../timer/timer.h"
#include "wifi.h"
#include "wifinet.h"
static int subscriptionID = 0;

/** \brief wifi_init
 *
 * \return wifi_err_t
 *
 */
wifi_err_t wifi_init()
{
   wifi_err_t err = WIFI_ERROR_NONE;
   return err;
}

void wifiEventHandler(subscriptionEvent_t event);

/** \brief wifi_connect
 *
 * \return wifi_err_t
 *
 */
wifi_err_t wifi_connect()
{
   int err = WIFI_ERROR_NONE;

   // Enables Wi-Fi STA (Station) mode
   cyw43_arch_enable_sta_mode();
   netif_set_hostname(netif_default, "smarthp");

   // Connect to the WiFI network w timeout
   if(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
      err = WIFI_TIMEOUT;
   else
      timer_subscribe(timer_10000mS, &wifiEventHandler, &subscriptionID);

   return err;
}


int wifi_reconnect()
{
   // Not really sure how to do this exactly. For now I let the watchdog reset the board. I think
   return 0;
}
//#define CYW43_LINK_DOWN         (0)     ///< link is down
//#define CYW43_LINK_JOIN         (1)     ///< Connected to wifi
//#define CYW43_LINK_NOIP         (2)     ///< Connected to wifi, but no IP address
//#define CYW43_LINK_UP           (3)     ///< Connected to wifi with an IP address
//#define CYW43_LINK_FAIL         (-1)    ///< Connection failed
//#define CYW43_LINK_NONET        (-2)    ///< No matching SSID found (could be out of range, or down)
//#define CYW43_LINK_BADAUTH      (-3)    ///< Authenticatation failure
void wifiEventHandler(subscriptionEvent_t event)
{
//   const char* linkstatus[] = {"LINK_DOWN", "LINK_JOIN", "LINK_NOIP", "LINK_UP"};
//   printf("tcpip linkstatus %s\n", linkstatus[cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA)]);
//   printf("wifi linkstatus %s\n", linkstatus[cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA)]);
   int32_t rssi ;
   cyw43_wifi_get_rssi (&cyw43_state, &rssi);
   printf("rssi=%d\n", rssi) ;
//   printf("Mask: %s\n", ip4addr_ntoa(netif_ip_netmask4(netif_default)));
//   printf("AP_channel %d\n\r", cyw43_state.ap_channel);
//   printf("Gateway: %s\n", ip4addr_ntoa(netif_ip_gw4(netif_default)));
//   printf("Host Name: %s\n", netif_get_hostname(netif_default));
}
