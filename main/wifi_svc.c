#include "wifi_svc.h"

static bool auto_reconnect = true;

/* FreeRTOS event group to signal when we are connected & ready to make a
 * request */
EventGroupHandle_t wifi_event_group;

static esp_err_t event_handler(void *ctx, system_event_t *event) {
  switch (event->event_id) {
  case SYSTEM_EVENT_STA_START:
    esp_wifi_connect();
    break;
  case SYSTEM_EVENT_STA_CONNECTED:
    /* enable ipv6 */
    tcpip_adapter_create_ip6_linklocal(TCPIP_ADAPTER_IF_STA);
    break;
  case SYSTEM_EVENT_STA_GOT_IP:
    xEventGroupSetBits(wifi_event_group, IP4_CONNECTED_BIT);
    break;
  case SYSTEM_EVENT_AP_STA_GOT_IP6:
    xEventGroupSetBits(wifi_event_group, IP6_CONNECTED_BIT);
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
    /* This is a workaround as ESP32 WiFi libs don't currently
       auto-reassociate. */
    if (auto_reconnect) {
      esp_wifi_connect();
    }
    xEventGroupClearBits(wifi_event_group,
                         IP4_CONNECTED_BIT | IP6_CONNECTED_BIT);
    break;
  default:
    break;
  }
  mdns_handle_system_event(ctx, event);
  return ESP_OK;
}

void initialise_wifi(void) {
  tcpip_adapter_init();
  wifi_event_group = xEventGroupCreate();
  ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  wifi_config_t wifi_config = {
      .sta =
          {
              .ssid = EXAMPLE_WIFI_SSID, .password = EXAMPLE_WIFI_PASS,
          },
  };
  ESP_LOGI("WIFI", "Setting WiFi configuration SSID %s...",
           wifi_config.sta.ssid);
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());
}