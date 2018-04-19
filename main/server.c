#include "server.h"
#include "index.html.h"

struct mg_mgr mgr;
struct mg_connection *nc;
struct mg_bind_opts bind_opts;

#define MG_LISTEN_ADDR "80"

static void mg_ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  static const char *reply_fmt = "HTTP/1.0 200 OK\r\n"
                                 "Connection: close\r\n"
                                 "Content-Type: text/html\r\n"
                                 "\r\n"
                                 "%s\n";
  static const char *json_fmt = "HTTP/1.0 200 OK\r\n"
                                "Connection: close\r\n"
                                "Content-Type: application/json\r\n"
                                "\r\n"
                                "%s\n";
  struct http_message *hm = (struct http_message *)ev_data;

  switch (ev) {
  case MG_EV_ACCEPT: {
    char addr[32];
    mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
                        MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
    // printf("Connection %p from %s\n", nc, addr);
    break;
  }
  case MG_EV_HTTP_REQUEST: {
    // REST-API
    // message of the day (mod) route
    if (mg_vcmp(&hm->uri, "/mod") == 0) {
      char mod[100], buf[100];
      mg_get_http_var(&hm->body, "mod", mod, sizeof(mod));
      ESP_LOGI("server", "received mod: %s", mod);
      snprintf(buf, 100, "{ \"mod\": \"%s\"}%c", mod, '\0');
      mg_printf(nc, json_fmt, buf);
      nc->flags |= MG_F_SEND_AND_CLOSE;
    } else {
      char addr[32];
      mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
                          MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
      printf("HTTP request from %s: %.*s %.*s\n", addr, (int)hm->method.len,
             hm->method.p, (int)hm->uri.len, hm->uri.p);
      mg_printf(nc, reply_fmt, main_index_html);
      nc->flags |= MG_F_SEND_AND_CLOSE;
    }
    break;
  }
  case MG_EV_CLOSE: {
    printf("Connection %p closed\n", nc);
    break;
  }
  }
}

static void main_task(void *pvParameters) {
  ESP_LOGI("server", "server starting");
  while (1) {
    mg_mgr_poll(&mgr, 1000);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void start_server() {
  /* Wait for the callback to set the CONNECTED_BIT in the event group. */
  xEventGroupWaitBits(wifi_event_group, IP4_CONNECTED_BIT | IP6_CONNECTED_BIT,
                      false, true, portMAX_DELAY);

  ESP_LOGI("server", "Starting web-server on port %s\n", MG_LISTEN_ADDR);

  mg_mgr_init(&mgr, NULL);

  nc = mg_bind(&mgr, MG_LISTEN_ADDR, mg_ev_handler);
  if (nc == NULL) {
    ESP_LOGE("server", "Error setting up listener!\n");
    return;
  }
  mg_set_protocol_http_websocket(nc);

  xTaskCreate(&main_task, "main_task", 4096, NULL, 5, NULL);
}