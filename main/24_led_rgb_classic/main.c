#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

/**
 * LED RGB indicator
 * Anodo comun enciende con un 0 en la salida
 */
#define LED_RGB_R_GPIO       GPIO_NUM_7
#define LED_RGB_G_GPIO       GPIO_NUM_6
#define LED_RGB_B_GPIO       GPIO_NUM_5

static const char *TAG = "Led RBG common anode";

void powerOff();

void app_main(void)
{

    // LEDs
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_RGB_R_GPIO) | (1ULL << LED_RGB_G_GPIO) | (1ULL << LED_RGB_B_GPIO),
        .mode = GPIO_MODE_OUTPUT,      
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    powerOff();

    while (true)
    {
        // 1. RED
        powerOff();
        ESP_LOGI(TAG, "1. Red");
        gpio_set_level( LED_RGB_R_GPIO, false);
        vTaskDelay(pdMS_TO_TICKS(1500));

        // 2. Green
        powerOff();
        ESP_LOGI(TAG, "2. Green");
        gpio_set_level( LED_RGB_G_GPIO, false);
        vTaskDelay(pdMS_TO_TICKS(1500));

        // 3. Blue
        powerOff();
        ESP_LOGI(TAG, "3. Blue");
        gpio_set_level( LED_RGB_B_GPIO, false);
        vTaskDelay(pdMS_TO_TICKS(1500));

        // 4. RED + Green
        powerOff();
        ESP_LOGI(TAG, "4. Red + Green");
        gpio_set_level( LED_RGB_R_GPIO, false);
        gpio_set_level( LED_RGB_G_GPIO, false);
        vTaskDelay(pdMS_TO_TICKS(1500));

        // 5. RED + Blue
        powerOff();
        ESP_LOGI(TAG, "5. Red + Blue");
        gpio_set_level( LED_RGB_R_GPIO, false);
        gpio_set_level( LED_RGB_G_GPIO, false);
        vTaskDelay(pdMS_TO_TICKS(1500));

        // 6. Blue + Green
        powerOff();
        ESP_LOGI(TAG, "6. Blue + Green");
        gpio_set_level( LED_RGB_B_GPIO, false);
        gpio_set_level( LED_RGB_G_GPIO, false);
        vTaskDelay(pdMS_TO_TICKS(1500));

        // 7. RED + Green + Blue
        powerOff();
        ESP_LOGI(TAG, "7. Red + Green + Blue");
        gpio_set_level( LED_RGB_R_GPIO, false);
        gpio_set_level( LED_RGB_G_GPIO, false);
        gpio_set_level( LED_RGB_B_GPIO, false);
        vTaskDelay(pdMS_TO_TICKS(1500));

        ESP_LOGI(TAG, "Power off");
        powerOff();
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
  
}

void powerOff() {
    // All off
    gpio_set_level( LED_RGB_R_GPIO, true);
    gpio_set_level( LED_RGB_G_GPIO, true);
    gpio_set_level( LED_RGB_B_GPIO, true);
}