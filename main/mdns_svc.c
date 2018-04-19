#include "mdns_svc.h"

void initialise_mdns(void) {
  // initialize mDNS
  ESP_ERROR_CHECK(mdns_init());
  // set mDNS hostname (required if you want to advertise services)
  ESP_ERROR_CHECK(mdns_hostname_set(EXAMPLE_MDNS_HOSTNAME));
  // set default mDNS instance name
  ESP_ERROR_CHECK(mdns_instance_name_set(EXAMPLE_MDNS_INSTANCE));

  // structure with TXT records
  mdns_txt_item_t serviceTxtData[3] = {
      {"board", "esp32"}, {"u", "user"}, {"p", "password"}};

  // initialize service
  ESP_LOGI("MDNS", "Initialize service...");
  ESP_ERROR_CHECK(mdns_service_add("ESP32-WebServer", "_http", "_tcp", 80,
                                   serviceTxtData, 3));
}