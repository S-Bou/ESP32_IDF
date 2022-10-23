/**
 *******************************************************************************************************************************************************
  * @file    GPIO_Config.c
  * @author  Bou
  * @version V1.0.0
  * @date    22-04-2022
  * @brief   This file provides...
  *******************************************************************************************************************************************************
 */
#include "GPIO_Config.h"
/********************************************************************************************************************************************************/

/********************************************************************************************************************************************************/
/**
  * @brief  This function...
  * @param  None
  * @retval None
  */
void configure_led(void)
{
    ESP_LOGI(SPP_TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(GREEN_LED);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(GREEN_LED, GPIO_MODE_OUTPUT);
    /* Set OFF the led */
    gpio_set_level(GREEN_LED, OFF);
}
/********************************************************************************************************************************************************/
/**
  * @brief  This function...
  * @param  None
  * @retval None
  */
void DoSome(char dataOne[])
{
    if(dataOne[1] == 'n'){
        gpio_set_level(GREEN_LED, ON);


    }else if(dataOne[2] == 'f'){
        gpio_set_level(GREEN_LED, OFF);

    }

    float datafloat = 4.1264;

    char temp[32];
    snprintf(temp, sizeof(temp), "%.3f", datafloat);

    Write_BT_SPP(temp);

}
/********************************************************************************************************************************************************/





