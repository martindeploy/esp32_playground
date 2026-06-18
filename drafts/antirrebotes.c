// From Telegram ESP32
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "driver/ledc.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"

static const char *TAG = "RGB";
xSemaphoreHandle GlobalKey = 0;
uint8_t Enable_isr = 1;

#define STACK_SIZE 1024 * 2
#define BLINK_GPIO 13
#define CONFIG_BLINK_LED_RMT_CHANNEL 0
#define RGB_MAX_QTTY 64
#define Default_Color 0
#define Default_Dimmer_Level 5

#define Button_Dimmer_Up 22
#define Button_Dimmer_Dowm 21
#define Button_Mode 19
#define Button_Color 18


void isr_handler_button_dimemer_up(void *args);
void isr_handler_button_dimemer_down(void *args);
void isr_handler_button_mode(void *args);
void isr_handler_button_color(void *args);


static void init_irs(void)
{
    gpio_config_t pGPIOConfig;
    pGPIOConfig.pin_bit_mask = (1ULL << Button_Dimmer_Up) | (1ULL << Button_Dimmer_Dowm) | (1ULL << Button_Color) | (1ULL << Button_Mode);
    pGPIOConfig.mode = GPIO_MODE_DEF_INPUT;
    pGPIOConfig.pull_up_en = GPIO_PULLUP_ENABLE;
    pGPIOConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    pGPIOConfig.intr_type = GPIO_INTR_NEGEDGE;

    gpio_config(&pGPIOConfig);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(Button_Dimmer_Up, isr_handler_button_dimemer_up, NULL);
    gpio_isr_handler_add(Button_Dimmer_Dowm, isr_handler_button_dimemer_down, NULL);
    gpio_isr_handler_add(Button_Color, isr_handler_button_color, NULL);
    gpio_isr_handler_add(Button_Mode, isr_handler_button_mode, NULL);

    ESP_LOGI(TAG, "Init isr completed");
}

void isr_handler_button_mode(void *args)
{
    if (Enable_isr == 1)
    {
        xSemaphoreGive(GlobalKey);
        //code
    }
}

void isr_handler_button_color(void *args)
{
    if (Enable_isr == 1)
    {
        xSemaphoreGive(GlobalKey);

       //code
    }
}

void isr_handler_button_dimemer_up(void *args)
{
    if (Enable_isr == 1)
    {
        xSemaphoreGive(GlobalKey);
        //code
    }
}

void isr_handler_button_dimemer_down(void *args)
{
    if (Enable_isr == 1)
    {
        xSemaphoreGive(GlobalKey);
        //code
    }
}

static void vTask_Evaluate_irs(void *pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(GlobalKey, portMAX_DELAY))
        {
            Enable_isr = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(200));
        Enable_isr = 1;
    }
}

static void create_tasks(void)
{
    static uint8_t ucParameterToPass;
    TaskHandle_t xHandle = NULL;
   
    xTaskCreate(vTask_Evaluate_irs,
                "vTask_Evaluate_irs",
                STACK_SIZE,
                &ucParameterToPass,
                1,
                &xHandle);
}

void app_main(void)
{
    GlobalKey = xSemaphoreCreateBinary();    
    init_irs();   
    create_tasks();
}
