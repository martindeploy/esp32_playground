/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#ifndef LED_H
#define LED_H

/* Includes */
/* ESP APIs */
#include "driver/gpio.h"

/* Defines */

// #define BLINK_GPIO GPIO_NUM_2 // ESP32
#define BLINK_GPIO GPIO_NUM_8 // ESP32C3 -> logica inversa off=disconnected on= connected


/* Public function declarations */
uint8_t get_led_state(void);
void led_on(void);
void led_off(void);
void led_init(void);

#endif // LED_H
