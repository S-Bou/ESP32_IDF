/*
 * SPDX-FileCopyrightText: 2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

//ADC Channels
#define ADC1_EXAMPLE_CHAN0          ADC1_CHANNEL_6               //ADC1 channel 6 is GPIO34

//ADC Attenuation
#define ADC_EXAMPLE_ATTEN           ADC_ATTEN_DB_11

//ADC Calibration
#define ADC_EXAMPLE_CALI_SCHEME     ESP_ADC_CAL_VAL_EFUSE_VREF

static uint32_t reading;

static const char *TAG = "ADC SINGLE";

static esp_adc_cal_characteristics_t adc1_chars;

//Resistencia fija del divisor de tensión
float R1 = 100000;               
float logR2, R2, TEMPERATURA;
//Coeficientes de S-H
float c1 = 2.114990448e-03, c2 = 0.3832381228e-04, c3 = 5.228061052e-07;

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

void app_main(void)
{
    uint32_t voltage = 0;
    bool cali_enable = adc_calibration_init();

    //ADC1 config
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_EXAMPLE_CHAN0, ADC_EXAMPLE_ATTEN));

    while (1) {
        reading = adc1_get_raw(ADC1_EXAMPLE_CHAN0);
        //ESP_LOGI(TAG, "raw  data: %d", reading);
        if (cali_enable) {
            voltage = esp_adc_cal_raw_to_voltage(reading, &adc1_chars);
            ESP_LOGI(TAG, "cali data: %d mV", voltage);
            //Conversion de tensión a resistencia
            R2 = R1 * (3300.0 / (float)voltage - 1.0); 
            logR2 = log(R2);            // logaritmo de R2 necesario para ecuación
            TEMPERATURA = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));     // ecuación S-H
            TEMPERATURA = TEMPERATURA - 273.15;   // Kelvin a Centigrados (Celsius)

            ESP_LOGI("Temperatura", "Grados: %.2f ºC", TEMPERATURA);
        }
        vTaskDelay(pdMS_TO_TICKS(2000)); //Calculate the number of ticks from a time specified in milliseconds 

    }
}
