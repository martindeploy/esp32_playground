#include "driver/gpio.h"
#include "esp_log.h"
#include "iot_button.h"
#include "button_gpio.h"

/**
 * https://docs.espressif.com/projects/esp-iot-solution/en/latest/input_device/button.html
 * Ejemplo utilizando la biblioteca espressif/button
 * 
 */

// Push button uno para cada direccion
#define UP_BUTTON_GPIO       GPIO_NUM_3
#define DOWN_BUTTON_GPIO     GPIO_NUM_21
#define LEFT_BUTTON_GPIO     GPIO_NUM_1
#define RIGHT_BUTTON_GPIO    GPIO_NUM_0

static const char *TAG = "21_gpio_button";

typedef enum { 
    UP = 0, 
    DOWN, 
    LEFT, 
    RIGHT } id_direction_t;


static void button_single_click_cb(void *arg, void *usr_data)
{
    id_direction_t direction = *(id_direction_t *)usr_data;
    switch (direction) {
        case UP:
            ESP_LOGW(TAG, "BUTTON_SINGLE_CLICK > UP");
            break;
        case DOWN:
            ESP_LOGW(TAG, "BUTTON_SINGLE_CLICK > DOWN");
            break;
        case LEFT:
            ESP_LOGW(TAG, "BUTTON_SINGLE_CLICK > LEFT");
            break;
        case RIGHT:
            ESP_LOGW(TAG, "BUTTON_SINGLE_CLICK > RIGHT");
            break;
        default:
            break;
    }
}

static void button_double_click_cb(void *arg, void *usr_data)
{
    ESP_LOGI(TAG, "BUTTON_DOUBLE_CLICK");
}

static void button_long_press_cb(void *arg, void *usr_data)
{
    ESP_LOGI(TAG, "BUTTON_LONG_PRESS_START");
}


void app_main()
{
    /**
     * Config de GPIO
     */
    // Up button
    const button_config_t btn_cfg_up = {0};
    const button_gpio_config_t btn_gpio_cfg_up = {
        .gpio_num = UP_BUTTON_GPIO,
        .active_level = 0,
        .enable_power_save = false,
    };
    button_handle_t handle_btn_up = NULL;
    ESP_ERROR_CHECK(iot_button_new_gpio_device(&btn_cfg_up, &btn_gpio_cfg_up, &handle_btn_up));

    // RIght button
    const button_config_t btn_cfg_right = {0};
    const button_gpio_config_t btn_gpio_cfg_right = {
        .gpio_num = RIGHT_BUTTON_GPIO,
        .active_level = 0,
        .enable_power_save = false,
    };
    button_handle_t handle_btn_right = NULL;
    ESP_ERROR_CHECK(iot_button_new_gpio_device(&btn_cfg_right, &btn_gpio_cfg_right, &handle_btn_right));
    
    /**
     * Callbacks
     */
    static id_direction_t id_direction_up = UP; 
    iot_button_register_cb(handle_btn_up, BUTTON_SINGLE_CLICK, NULL, button_single_click_cb, &id_direction_up);

    static id_direction_t id_direction_right = RIGHT; 
    iot_button_register_cb(handle_btn_right, BUTTON_SINGLE_CLICK, NULL, button_single_click_cb, &id_direction_right);

    iot_button_register_cb(handle_btn_up, BUTTON_DOUBLE_CLICK, NULL, button_double_click_cb, NULL);
    iot_button_register_cb(handle_btn_up, BUTTON_LONG_PRESS_START, NULL, button_long_press_cb, NULL);


}
