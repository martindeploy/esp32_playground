#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"


// Ejemplo task por LED
#define LED_BOARD   2

// Tiempo en ms
#define DELAY_TIME_BOARD  500  //ms

/**
 * Prototipos de funciones
 */
esp_err_t init_leds(void);
esp_err_t blink_blocking(gpio_num_t gpioNum, int millisecons);

/**
 * Inicializar
 */
esp_err_t init_leds(void)
{  
  // 1. Forma simple de configurar I/O
  // gpio_reset_pin(LED_BOARD);
  // gpio_set_direction(LED_BOARD, GPIO_MODE_OUTPUT);


  // 2 .Forma completa con toda la struct
  gpio_config_t io_conf = {
    .pin_bit_mask = (1ULL << LED_BOARD),   // Select GPIO 2
    .mode = GPIO_MODE_OUTPUT,              // Set as output
    .pull_up_en = GPIO_PULLUP_DISABLE,     // Disable pull-up
    .pull_down_en = GPIO_PULLDOWN_DISABLE, // Disable pull-down
    .intr_type = GPIO_INTR_DISABLE         // Disable interrupts
  };
  gpio_config(&io_conf);



  return ESP_OK;
}

/**
 * Delay bloqueante, equivalente a delay(500) de Arduino
 */
esp_err_t blink_blocking(gpio_num_t gpioNum, int milliseconds) {
  ESP_LOGI("blink_blocking", "Turning On GIPIO%d", gpioNum);
  gpio_set_level(gpioNum, true);
  vTaskDelay(pdMS_TO_TICKS(milliseconds)); 
  ESP_LOGI("blink_blocking", "Turning Off GPIO%d", gpioNum);
  gpio_set_level(gpioNum, false);
  vTaskDelay(pdMS_TO_TICKS(milliseconds));

  return ESP_OK;
}


void app_main(void)
{
  init_leds();

  while (true)
  {
    blink_blocking(LED_BOARD, DELAY_TIME_BOARD); 
  }
  
}
