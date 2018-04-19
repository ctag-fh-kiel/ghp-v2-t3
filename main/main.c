/* MDNS-SD Query and advertise Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "globals.h"
#include "wifi_svc.h"
#include "mdns_svc.h"
#include "server.h"



void app_main()
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    initialise_mdns();
    initialise_wifi();
    start_server();
}
