#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ssd1306.h"
#include "font8x8_basic.h"

/*
 You have to set this config value with menuconfig
 CONFIG_INTERFACE

 for i2c
 CONFIG_MODEL
 CONFIG_SDA_GPIO
 CONFIG_SCL_GPIO
 CONFIG_RESET_GPIO
*/

#define tag "SSD1306"

	SSD1306_t dev;

float tempe = 12.34;

void Config_SSD1306(void){

#if CONFIG_I2C_INTERFACE
	ESP_LOGI(tag, "INTERFACE is i2c");
	ESP_LOGI(tag, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(tag, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	ESP_LOGI(tag, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_I2C_INTERFACE

#if CONFIG_FLIP
	dev._flip = true;
	ESP_LOGW(tag, "Flip upside down");
#endif

#if CONFIG_SSD1306_128x64
	ESP_LOGI(tag, "Panel is 128x64");
	ssd1306_init(&dev, 128, 64);
#endif // CONFIG_SSD1306_128x64

	ssd1306_clear_screen(&dev, false);
	ssd1306_contrast(&dev, 0xff);
    ssd1306_display_text_x3(&dev, 3, "Hello", 5, false);

    vTaskDelay(3000 / portTICK_PERIOD_MS);
    ssd1306_clear_screen(&dev, false);
}

void app_main(void)
{
	Config_SSD1306();

	ssd1306_display_text(&dev, 0, "                 ", 17, true);
	ssd1306_display_text(&dev, 1, "     CECOTEC     ", 17, true);
	ssd1306_display_text(&dev, 2, "                 ", 17, true);
	ssd1306_display_text(&dev, 3, "                 ", 17, true);
	
	char buffer[20];
    snprintf(buffer, sizeof buffer, "  Temp: %.2f", tempe);
	ssd1306_display_text(&dev, 4, buffer, sizeof buffer, true);

	ssd1306_display_text(&dev, 5, "                 ", 17, true);
	ssd1306_display_text(&dev, 6, "                 ", 17, true);
	ssd1306_display_text(&dev, 7, "                 ", 17, true);

	vTaskDelay(3000 / portTICK_PERIOD_MS);
	
	// Fade Out
	//ssd1306_fadeout(&dev);

	//esp_restart(); // Restart module
	
}

