/*
 * SPDX-FileCopyrightText: 2021-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#ifndef _ESP_HID_GAP_H_
#define _ESP_HID_GAP_H_

#define HIDD_IDLE_MODE 0x00
#define HIDD_BLE_MODE 0x01
#define HIDD_BT_MODE 0x02
#define HIDD_BTDM_MODE 0x03

#define HID_DEV_MODE HIDD_BLE_MODE

// Config
#define CONFIG_EXAMPLE_MOUSE_ENABLE 1
#define CONFIG_EXAMPLE_HID_DEVICE_ROLE 3



#include "esp_err.h"
#include "esp_log.h"

#include "esp_bt.h"
#include "esp_hid_common.h"



esp_err_t esp_hid_gap_init(uint8_t mode);
esp_err_t esp_hid_gap_deinit(void);

esp_err_t esp_hid_ble_gap_adv_init(uint16_t appearance, const char *device_name);
esp_err_t esp_hid_ble_gap_adv_start(void);


#endif /* _ESP_HIDH_GAP_H_ */
