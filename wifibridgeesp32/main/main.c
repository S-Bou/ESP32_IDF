#include <stdio.h>

#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "threading_alt.h"
#include "mbedtls/threading.h"
#include "iot_crypto.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "dummy/dummyControl.h"
#include "console/dummy_console.h"
#include "uart/uartESP32.h"

int app_main()
{
    ESP_LOGI("MAIN FUNCTION", "Start DUMMY program\n");

    esp_event_loop_create_default();
    CRYPTO_Init();
    esp_err_t err = nvs_flash_init();

    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
        ESP_LOGI("MAIN FUNCTION", "Init NVS flash returned ERROR\n");
        ESP_ERROR_CHECK(nvs_flash_erase());
    }

    err = esp_netif_init();
    if (err != ESP_OK) 
    {
        ESP_LOGI("MAIN FUNCTION", "Init netif returned ERROR\n");
    }

    initDummyConsole();
    initDummyFirmware();
    uartInit();

    return 0;
}
