#include "wirelessCommunication/linkingManager.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#define UNIVERSALDUMMY_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define UNIVERSALDUMMY_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define UNIVERSALDUMMY_WIFI_CHANNEL   CONFIG_ESP_WIFI_CHANNEL
#define MAX_STA_CONN                  CONFIG_ESP_MAX_STA_CONN
#define LINKING_PROCESS_TIME_OUT 600

static const char *TAG = "wifi softAP";
int firstTime = 0;
TaskHandle_t softAPThreadHandle = NULL;

int8_t provisionStatus = PROVISION_PROCESS_READY;

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) 
    {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } 
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) 
    {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

void wifi_init_softap(void)
{
    ESP_LOGI(TAG, "Set UP Soft AP!");

    if(firstTime == 0)
    {
        esp_netif_create_default_wifi_ap();
        firstTime = 1;
    }
    
    ESP_LOGI(TAG, "Init Wi-Fi Config default!");
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_LOGI(TAG, "Init Wi-Fi!");
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_config = 
    {
        .ap = {
            .ssid = UNIVERSALDUMMY_WIFI_SSID,
            .ssid_len = strlen(UNIVERSALDUMMY_WIFI_SSID),
            .channel = UNIVERSALDUMMY_WIFI_CHANNEL,
            .max_connection = MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .password = UNIVERSALDUMMY_WIFI_PASS
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
            UNIVERSALDUMMY_WIFI_SSID, UNIVERSALDUMMY_WIFI_PASS, UNIVERSALDUMMY_WIFI_CHANNEL);
}

void wakeUpSoftAP()
{
    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();
    provisionStatus = PROVISION_PROCESS_LINKING;
}