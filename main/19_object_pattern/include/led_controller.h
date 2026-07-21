#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Struct
 */
struct led_controller {
    uint8_t gpio_pin;       // pin GPIO del led
    bool is_on;             // estado actual
    uint32_t blink_period;  // period de blink
    uint32_t last_togle;    // ultimo tiempo de cambio
};

/**
 * Init LED object
 */
int led_controller_init(struct led_controller *self, uint8_t gpio_pin);

/**
 *  Destroy
 */
void led_controller_deinit(struct led_controller *self);

/**
 * TurnOn
 */
void led_controller_turn_on(struct led_controller *self);

/**
 * TurnOff
 */
void led_controller_turn_off(struct led_controller *self);

/**
 * Toglle
 */
void led_controller_toggle(struct led_controller *self);

/**
 * Configura el periodo de parpadeo
 */
void led_controller_set_blink_period(struct led_controller *self, uint32_t period_ms);

/**
 * Update blink time
 */
void led_controller_update(struct led_controller *self, uint32_t current_time);

#endif