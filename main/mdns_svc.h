#pragma once

#include "esp_log.h"
#include "mdns.h"

#define EXAMPLE_MDNS_HOSTNAME CONFIG_MDNS_HOSTNAME
#define EXAMPLE_MDNS_INSTANCE CONFIG_MDNS_INSTANCE

void initialise_mdns(void);