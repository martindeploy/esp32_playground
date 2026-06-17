#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"

// Optputs
#define LED_BOARD   2
#define LED_RED     27
#define LED_GREEN   33


static const char *TAG = "02_blink_timers";


/**
 * Prototipos de funciones
 */
esp_err_t initInputOutputs(void);


// Estado de LEDS
uint8_t ledBoardState = false;
uint8_t ledRedState = false;
uint8_t ledGreenState = false;


/**
 * Inicializar I/O
 */
esp_err_t initInputOutputs(void)
{  
  // LED_BOARD
  gpio_reset_pin(LED_BOARD);
  gpio_set_direction(LED_BOARD, GPIO_MODE_OUTPUT);
  
  // LED_GREEN
  gpio_reset_pin(LED_GREEN);
  gpio_set_direction(LED_GREEN, GPIO_MODE_OUTPUT);
  
  // LED_RED
  gpio_reset_pin(LED_RED);
  gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
  
  
  return ESP_OK;
}



void app_main(void)
{
  initInputOutputs();
  
}
