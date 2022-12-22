#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
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

//ADC Channels
#define ADC1_EXAMPLE_CHAN0          ADC1_CHANNEL_6               //ADC1 channel 6 is GPIO34

//ADC Attenuation
#define ADC_EXAMPLE_ATTEN           ADC_ATTEN_DB_11

//ADC Calibration
#define ADC_EXAMPLE_CALI_SCHEME     ESP_ADC_CAL_VAL_EFUSE_VREF

static uint32_t reading;

static const char *TAG = "ADC single ";

static esp_adc_cal_characteristics_t adc1_chars;

//Valores fijos del circuito
float rAux = 100000.0;
float vcc = 5.0;
float beta = 4190.0;
float temp0 = 298.0;
float r0 = 100000.0;

//Variables usadas en el cálculo
float vm = 0.0;
float rntc = 0.0;
float temperaturaK = 0.0;
float tempeCelsius = 0.0;

void TakeTemperature(uint32_t voltsIN)
{
    //Bloque de cálculo
    //vm=(vcc / 1024)*( voltsIN );                    //Calcular tensión en la entrada
    vm = voltsIN/1000;                                //Calcular tensión en la entrada
    rntc = rAux / ((vcc/vm)-1);                       //Calcular la resistencia de la NTC
    ESP_LOGI("Termistor  ", "%.2f Ohms", rntc);

    temperaturaK = beta/(log(rntc/r0)+(beta/temp0));  //Calcular la temperatura en Kelvin

    //Restar 273 para pasar a grados celsus
    tempeCelsius = temperaturaK - 273;
    ESP_LOGI("Temperatura", "%.2f °C", tempeCelsius);
}

static bool adc_calibration_init(void)
{
    esp_err_t ret;
    bool cali_enable = false;

    ret = esp_adc_cal_check_efuse(ADC_EXAMPLE_CALI_SCHEME);
    if (ret == ESP_ERR_NOT_SUPPORTED) {
        ESP_LOGW(TAG, "Calibration scheme not supported, skip software calibration");
    } else if (ret == ESP_ERR_INVALID_VERSION) {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else if (ret == ESP_OK) {
        cali_enable = true;
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_EXAMPLE_ATTEN, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    } else {
        ESP_LOGE(TAG, "Invalid arg");
    }

    return cali_enable;
}

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

        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // Fade Out
        ssd1306_fadeout(&dev);
        //ssd1306_clear_screen(&dev, false);
}

void app_main(void)
{
	Config_SSD1306();

    uint32_t voltage = 0;
    bool cali_enable = adc_calibration_init();

    //ADC1 config
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_EXAMPLE_CHAN0, ADC_EXAMPLE_ATTEN));

    while (1) {
        ESP_LOGW("", "---------------------------------------------------");
        reading = adc1_get_raw(ADC1_EXAMPLE_CHAN0);
        //ESP_LOGI(TAG, "raw  data: %d", reading);
        if (cali_enable) {
            voltage = esp_adc_cal_raw_to_voltage(reading, &adc1_chars);
            ESP_LOGI(TAG, "%d mV", voltage);

            TakeTemperature(voltage);

            ssd1306_display_text(&dev, 0, "                 ", 17, true);
			ssd1306_display_text(&dev, 1, "     CECOTEC     ", 17, true);
            ssd1306_display_text(&dev, 2, "                 ", 17, true);
			ssd1306_display_text(&dev, 3, "Temperatura:     ", 17, true);
            ssd1306_display_text(&dev, 4, "                 ", 17, true);

			char buffer[20];
		    snprintf(buffer, sizeof buffer, "     %.2f", tempeCelsius);

			ssd1306_display_text(&dev, 5, buffer, sizeof buffer, true);
			ssd1306_display_text(&dev, 6, "                 ", 17, true);
			ssd1306_display_text(&dev, 7, "                 ", 17, true);

			vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        //vTaskDelay(pdMS_TO_TICKS(2000)); //Calculate the number of ticks from a time specified in milliseconds 
    }

	// Fade Out
	//ssd1306_fadeout(&dev);

	//esp_restart(); // Restart module
	
}

