#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"




/**
 * Lectura de Joystick HW-504
 * 
 * Conexiones con ESP32C3
 * VRx -> A0 (analog) ADC_CHANNEL_0
 * VRy -> A1 (analog) ADC_CHANNEL_1
 * SW -> GPIO2 (digital-pulsador)
*/



static const char *TAG = "15_joystick_HW-504";

#define JOY_THRESHOLD_LOW   1000
#define JOY_THRESHOLD_HIGH  3000

typedef enum {
    JOY_CENTER,
    JOY_LEFT,
    JOY_RIGHT,
    JOY_UP,
    JOY_DOWN,
} joystick_dir_t;

/**
 * Prototipos de funciones
 */
esp_err_t init_analog_inputs(void);
esp_err_t init_digital_input(void);
void print_adc_values(void);
void print_sw_value(void);

joystick_dir_t get_direction(int x, int y);

int vRx = 0;
int vRy = 0;
static adc_oneshot_unit_handle_t adc1_handle;

void app_main(void)
{
    init_analog_inputs();
    init_digital_input();

    while (1)
    {
        print_adc_values();
        print_sw_value();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
}


esp_err_t init_analog_inputs(void)
{
    // ACD1
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc1_handle));

    adc_oneshot_chan_cfg_t chan_config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0, &chan_config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_1, &chan_config));

    return ESP_OK;
}

esp_err_t init_digital_input(void)
{
    // Pin para el boton
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << GPIO_NUM_2),   // Select GPIO 3
        .mode = GPIO_MODE_INPUT,              // Set as input
        .pull_up_en = GPIO_PULLUP_ENABLE,     // Enable pull-up
        .pull_down_en = GPIO_PULLDOWN_DISABLE, // Disable pull-down
        .intr_type = GPIO_INTR_DISABLE         // Disable interrupts
    };
    gpio_config(&io_conf);
    return ESP_OK;
}

/**
 * 
 */
void print_adc_values(void)
{
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &vRx));
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_1, &vRy));

    joystick_dir_t dir = get_direction(vRx, vRy);
    const char *dir_str[] = { "CENTER", "LEFT", "RIGHT", "UP", "DOWN" };
    ESP_LOGI(TAG, "vRx: %i  vRy: %i  -> %s", vRx, vRy, dir_str[dir]);
}

void print_sw_value(void)
{
    bool sw = gpio_get_level(GPIO_NUM_2);
    ESP_LOGI(TAG, "Sw: %i", sw);
}

joystick_dir_t get_direction(int x, int y)
{
    if (x < JOY_THRESHOLD_LOW)  return JOY_LEFT;
    if (x >= JOY_THRESHOLD_HIGH) return JOY_RIGHT;
    if (y < JOY_THRESHOLD_LOW)  return JOY_UP;
    if (y >= JOY_THRESHOLD_HIGH) return JOY_DOWN;
    return JOY_CENTER;
}