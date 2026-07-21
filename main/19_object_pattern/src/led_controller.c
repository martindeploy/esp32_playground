#include <string.h>
#include <driver/gpio.h>
#include "led_controller.h"

int led_controller_init(struct led_controller *self, uint8_t gpio_pin)
{
    // Limpiar memoria
    memset(self, 0, sizeof(*self));

    (*self).gpio_pin = gpio_pin;
    (*self).is_on = false;
    (*self).blink_period = 0;
    (*self).last_togle = 0;

    gpio_config_t io_config = {
        .pin_bit_mask = (1ULL << gpio_pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLDOWN_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    return gpio_config(&io_config);
}

void led_controller_deinit(struct led_controller *self)
{
    // Power Off
    led_controller_turn_off(self);

    gpio_reset_pin((*self).gpio_pin);

    memset(self, 0, sizeof(*self));
}

void led_controller_turn_on(struct led_controller *self)
{
    (*self).is_on = true;
    gpio_set_level( (*self).gpio_pin, true );
}

void led_controller_turn_off(struct led_controller *self)
{
    (*self).is_on = false;
    gpio_set_level( (*self).gpio_pin, false);
}

void led_controller_toggle(struct led_controller *self)
{
    if( (*self).is_on) {
        led_controller_turn_off(self);
    } else {
        led_controller_turn_on(self);
    }
}

void led_controller_set_blink_period(struct led_controller *self, uint32_t period_ms)
{
    (*self).blink_period = period_ms;
}

void led_controller_update(struct led_controller *self, uint32_t current_time)
{
    if( (*self).blink_period == 0) {
        return;
    }

    if( (current_time - (*self).last_togle) >= (*self).blink_period ) {
        led_controller_toggle(self);
        (*self).last_togle = current_time;
    }
}
