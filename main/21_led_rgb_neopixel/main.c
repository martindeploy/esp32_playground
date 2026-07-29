#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_strip.h"

#define LED_STRIP_BLINK_GPIO  48
// El LED integrado cuenta como una tira de 1 solo pixel
#define LED_STRIP_LED_NUMBERS 1

static const char *TAG = "RGB_DEV_KIT";
static led_strip_handle_t led_strip;

void configure_led(void)
{
    ESP_LOGI(TAG, "Configurando LED RGB direccionable...");
    
    /* Configuración de la tira LED */
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP_BLINK_GPIO,
        .max_leds = LED_STRIP_LED_NUMBERS,
        .led_model = LED_MODEL_WS2812,
        .flags.invert_out = false,
    };

    /* Configuración del periférico RMT */
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,
    };

    // Crear el objeto de control para el LED
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    
    // Asegurar que inicie apagado
    led_strip_clear(led_strip);
}

void app_main(void)
{
    // Inicializar el hardware
    configure_led();

    while (1) {
        // 1. Color Rojo (Valores: R, G, B de 0 a 255)
        ESP_LOGI(TAG, "LED: Rojo");
        led_strip_set_pixel(led_strip, 0, 255, 0, 0);
        led_strip_refresh(led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000));

        // 2. Color Verde
        ESP_LOGI(TAG, "LED: Verde");
        led_strip_set_pixel(led_strip, 0, 0, 255, 0);
        led_strip_refresh(led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000));

        // 3. Color Azul
        ESP_LOGI(TAG, "LED: Azul");
        led_strip_set_pixel(led_strip, 0, 0, 0, 255);
        led_strip_refresh(led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000));

        // 3. Color Rosa
        ESP_LOGI(TAG, "LED: Rosa");
        led_strip_set_pixel(led_strip, 0, 255, 28, 179);
        led_strip_refresh(led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000));

        // 3. Color Blanco
        ESP_LOGI(TAG, "LED: Blanco");
        led_strip_set_pixel(led_strip, 0, 255, 255, 255);
        led_strip_refresh(led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000));
        
        // 4. Apagar antes de reiniciar el ciclo
        led_strip_clear(led_strip);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
