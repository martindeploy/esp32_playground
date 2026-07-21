#include <freertos/FreeRTOS.h>
#include "led_controller.h"
#include <driver/gpio.h>


void app_main(void)
{
    struct led_controller led_builtin;
    
    led_controller_init(&led_builtin, GPIO_NUM_8);
    led_controller_set_blink_period(&led_builtin, 500);

    while (1)
    {
        // led_controller_turn_on(&led_builtin);
        // vTaskDelay(pdMS_TO_TICKS(500));
        // led_controller_turn_off(&led_builtin);
        // vTaskDelay(pdMS_TO_TICKS(500));

        // Obtener tiempo
        uint32_t current_time = xTaskGetTickCount() * portTICK_PERIOD_MS;

        // Update
        led_controller_update(&led_builtin, current_time);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    // 
    led_controller_deinit(&led_builtin);
  
}