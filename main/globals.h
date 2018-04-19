#pragma once
#include "esp_wifi.h"

#define IP4_CONNECTED_BIT BIT0
#define IP6_CONNECTED_BIT BIT1

extern EventGroupHandle_t wifi_event_group;