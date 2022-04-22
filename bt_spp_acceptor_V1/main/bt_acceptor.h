/**
 ********************************************************************************************************************************************************
 * @file     bt_acceptor.h
 * @brief    bt_acceptor header file
 * @version  V1.0.0
 * @date     22-04-2022
 ********************************************************************************************************************************************************
 */

#ifndef INC_BT_ACCEPTOR_H_
#define INC_BT_ACCEPTOR_H_

#ifdef __cplusplus
extern "C" {
#endif
/********************************************************************************************************************************************************/
#include <stdio.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "esp_log.h"
#include "GPIO_Config.h"

char *datarerceibed;
/********************************************************************************************************************************************************/

void BT_Config(void);
void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);

/********************************************************************************************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* INC_BT_ACCEPTOR_H_ */
