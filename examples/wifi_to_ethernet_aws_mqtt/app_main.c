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
#include <stdio.h>
#include "cmsis_os2.h"

extern void app_initialize (void);
extern int32_t socket_startup (void);
extern int32_t demo (void);

static const osThreadAttr_t ethernet_app_attr = {
  .stack_size = 4096U
};
  
static const osThreadAttr_t wifi_app_attr = {
  .stack_size = 4096U
  //.priority   = osPriorityBelowNormal
};

/*-----------------------------------------------------------------------------
 * Application main thread
 *----------------------------------------------------------------------------*/
static void ethernet_app_main (void *argument) {
  int32_t status;
  printf("## ETHNET MQTT Demo ##\r\n");
  
  ETHERNET_MQTT_thread();
}

static void wifi_app_main (void *argument) {
  int32_t status;
  
  printf("## WIFI TCP Server Demo ##\r\n");
  status = wifi_socket_startup();
  if (status == 0) {
    WIFI_TCP_AP_thread();
  }
}

/*-----------------------------------------------------------------------------
 * Application initialization
 *----------------------------------------------------------------------------*/
void app_initialize (void) {
  osThreadNew(ethernet_app_main, NULL, &ethernet_app_attr);
  osThreadNew(wifi_app_main, NULL, &wifi_app_attr);
}


