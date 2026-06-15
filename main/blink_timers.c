#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"


// Ejemplo task por LED
#define LED_BOARD   2
#define LED_RED     27
#define LED_GREEN   33
#define LED_YELLOW  21

// Tiempo en ms
#define DELAY_TIME_BOARD  500  //ms
#define DELAY_TIME_RED    1000 //ms


uint8_t led_state_red = 0;
uint8_t led_state_board = 0;


static const char *TAG = "gpio_example";


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
  // LED_BOARD
  gpio_reset_pin(LED_BOARD);
  gpio_set_direction(LED_BOARD, GPIO_MODE_OUTPUT);
  
  // LED_RED
  gpio_reset_pin(LED_RED);
  gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
  
  // LED_GREEN
  gpio_reset_pin(LED_GREEN);
  gpio_set_direction(LED_GREEN, GPIO_MODE_OUTPUT);

  // LED_YELLOW
  gpio_reset_pin(LED_YELLOW);
  gpio_set_direction(LED_YELLOW, GPIO_MODE_OUTPUT);
  
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
    blink_blocking(LED_BOARD, 500); 
  }
  

  
}
