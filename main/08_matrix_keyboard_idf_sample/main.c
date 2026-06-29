/*
 * SPDX-FileCopyrightText: 2020-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

 // SOLO VALIDO para ESP32-S3

#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "matrix_keyboard.h"

const static char *TAG = "08_matrix_keyboard_library";


// Filas 1 al 4 (salidas)
#define ROW_1   GPIO_NUM_3
#define ROW_2   GPIO_NUM_21
#define ROW_3   GPIO_NUM_19
#define ROW_4   GPIO_NUM_18

// Columnas 2 al 8  (entradas)
#define COL_1   GPIO_NUM_36
#define COL_2   GPIO_NUM_39
#define COL_3   GPIO_NUM_34
#define COL_4   GPIO_NUM_35

/**
 * @brief Matrix keyboard event handler
 * @note This function is run under OS timer task context
 */
esp_err_t example_matrix_kbd_event_handler(matrix_kbd_handle_t mkbd_handle, matrix_kbd_event_id_t event, void *event_data, void *handler_args)
{
    uint32_t key_code = (uint32_t)event_data;
    switch (event) {
    case MATRIX_KBD_EVENT_DOWN:
        ESP_LOGI(TAG, "press event, key code = %04"PRIx32, key_code);
        break;
    case MATRIX_KBD_EVENT_UP:
        ESP_LOGI(TAG, "release event, key code = %04"PRIx32, key_code);
        break;
    }
    return ESP_OK;
}

void app_main(void)
{
    matrix_kbd_handle_t kbd = NULL;
    // Apply default matrix keyboard configuration
    matrix_kbd_config_t config = MATRIX_KEYBOARD_DEFAULT_CONFIG();
    // Set GPIOs used by row and column line
    config.col_gpios = (int[]) {
        COL_1, COL_2, COL_3, COL_4
    };
    config.nr_col_gpios = 4;
    config.row_gpios = (int[]) {
        ROW_1, ROW_2, ROW_3, ROW_4
    };
    config.nr_row_gpios = 4;
    // Install matrix keyboard driver
    matrix_kbd_install(&config, &kbd);
    // Register keyboard input event handler
    matrix_kbd_register_event_handler(kbd, example_matrix_kbd_event_handler, NULL);
    // Keyboard start to work
    matrix_kbd_start(kbd);
}
