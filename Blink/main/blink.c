/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO_RED 5
#define BLINK_GPIO_GREEN 18
#define BLINK_GPIO_BLUE 19

#define MyTIME 100

void app_main(void)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_reset_pin(BLINK_GPIO_RED);
    gpio_reset_pin(BLINK_GPIO_GREEN);
    gpio_reset_pin(BLINK_GPIO_BLUE);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO_RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(BLINK_GPIO_GREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(BLINK_GPIO_BLUE, GPIO_MODE_OUTPUT);

    while(1) {
        /* Blink off (output low) */
        printf("Turning OFF the LED RED\n");
        gpio_set_level(BLINK_GPIO_RED, 1);
        vTaskDelay(MyTIME / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        printf("Turning ON the LED RED\n");
        gpio_set_level(BLINK_GPIO_RED, 0);
        vTaskDelay(MyTIME / portTICK_PERIOD_MS);

        /* Blink off (output low) */
        printf("Turning OFF the LED GREEN\n");
        gpio_set_level(BLINK_GPIO_GREEN, 1);
        vTaskDelay(MyTIME / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        printf("Turning ON the LED GREEN\n");
        gpio_set_level(BLINK_GPIO_GREEN, 0);
        vTaskDelay(MyTIME / portTICK_PERIOD_MS);

        /* Blink off (output low) */
        printf("Turning OFF the LED BLUE\n");
        gpio_set_level(BLINK_GPIO_BLUE, 1);
        vTaskDelay(MyTIME / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        printf("Turning ON the LED BLUE\n");
        gpio_set_level(BLINK_GPIO_BLUE, 0);
        vTaskDelay(MyTIME / portTICK_PERIOD_MS);
    }
}
