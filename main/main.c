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

static void main_task(void *pvParameters)
{
    /* Wait for the callback to set the CONNECTED_BIT in the event group. */
    xEventGroupWaitBits(wifi_event_group, IP4_CONNECTED_BIT | IP6_CONNECTED_BIT, false, true, portMAX_DELAY);
    ESP_LOGI("main", "Entering main task");
    while(1) {
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    initialise_mdns();
    initialise_wifi();
    xTaskCreate(&main_task, "main_task", 2048, NULL, 5, NULL);
}
