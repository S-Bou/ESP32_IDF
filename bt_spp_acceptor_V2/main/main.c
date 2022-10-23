/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "time.h"
#include "sys/time.h"
#include "bt_acceptor.h"
#include "GPIO_Config.h"

//extern char *datarerceibed;

void app_main(void)
{
    /* Configure the peripheral according to the LED type */
    configure_led();
    /* Configure blue-tooth connection */
    BT_Config();

}

