/* -----------------------------------------------------------------------------
 * Copyright (c) 2020 Arm Limited (or its affiliates). All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * -------------------------------------------------------------------------- */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "Driver_WiFi.h"

/* WiFi Driver Reference */
extern ARM_DRIVER_WIFI Driver_WiFi1;
static ARM_DRIVER_WIFI *wifi = &Driver_WiFi1;

extern void * WIFI_TCP_AP_thread(int socketNum, void * mac);

/* Access Point(Server) Settings */
#define SSID            "Wiz360_Test"
#define PASSWORD        "1234567890" //min. 8 character
#define SECURITY_TYPE   ARM_WIFI_SECURITY_WPA2

/* Access Point(Server) IP */
uint8_t Server_ip[4] = {192, 168, 36, 1};

/* Initialize and Power-on WiFi Module */
void wifi_init_power_ap (void) {

  //Initialize(cb_event), cb_event is a callback func to process CLIENT has CONNECTED
  //                      argument is ARM_WIFI_EVENT_AP_CONNECT, mac addr
  if(wifi->Initialize(NULL) != ARM_DRIVER_OK )
  {
    printf("wifi->initialize err\r\n");
  }
  
  if(wifi->PowerControl(ARM_POWER_FULL) !=ARM_DRIVER_OK )
  {
    printf("wifi->powerControl err\r\n");
  }
}

void wifi_set_options_ap(void)
{
  int32_t ret;
  /* Driver Options block */
  //uint8_t ap_ip[4] = {192, 168, 36, 1};       /* Server IP address */
  uint8_t ap_ip[4]={0,};
  uint8_t ap_gateway[4] = {192U, 168U, 36U, 1U};
  uint8_t ap_netmask[4] = {255U, 255U, 255U, 0U};
  uint8_t ap_dhcp_pool_start[4] = {192U, 168U, 36U, 100U};
  uint8_t ap_dhcp_pool_end[4] = {192U, 168U, 36U, 200U};
  uint32_t ap_dhcp_lease = 2000;
  //uint8_t dns[4] = {8U, 8U, 8U, 8U};   
  //uint8_t ap_mac[6] = {0x02, 0x08, 0xdc, 0x36, 0x07, 0x6d};

  memcpy(ap_ip, Server_ip, 4U);
  
  /* Driver Settings block */
  ret += wifi->SetOption (1U, ARM_WIFI_IP, ap_ip, 4U); //0U:station, 1U:Ap
  //ret = wifi->SetOption (1U, ARM_WIFI_MAC, ap_mac, 6U);
  ret += wifi->SetOption (1U, ARM_WIFI_IP_GATEWAY, ap_gateway, 4U);
  ret += wifi->SetOption (1U, ARM_WIFI_IP_SUBNET_MASK, ap_netmask, 4U);
  ret += wifi->SetOption (1U, ARM_WIFI_IP_DHCP_POOL_BEGIN, ap_dhcp_pool_start, 4U);
  ret += wifi->SetOption (1U, ARM_WIFI_IP_DHCP_POOL_END, ap_dhcp_pool_end, 4U);
  ret += wifi->SetOption (1U, ARM_WIFI_IP_DHCP_LEASE_TIME, ap_dhcp_lease, 4U);
  //ret = wifi->SetOption (1U, ARM_WIFI_IP_DNS1, dns, 4U);

  if (ret == 0U) {
    printf("WiFi SetOption succeeded!\r\n");
  } else {
    printf("WiFi SetOption error failed!\r\n");
  }
}


int32_t wifi_socket_startup (void) {
  ARM_WIFI_CONFIG_t config;
  int32_t ret;
  
  wifi_init_power_ap();

  osDelay (100U);

  memset((void *)&config, 0, sizeof(config));
  config.ssid     = SSID;
  config.pass     = PASSWORD;
  config.security = SECURITY_TYPE;
  config.ch       = 1U;

  ret = wifi->Activate(1U, &config);		//Interface (0 = Station, 1 = Access Point)
  if(ret == 0U) //ARM_DRIVER_OK == 0
  {
  	printf("WiFi Activated = %d\r\n", ret);
  } else {
    printf("WiFi Activate FAIL = %d\r\n",ret);
  }

  wifi_set_options_ap();

  return 0;
}
