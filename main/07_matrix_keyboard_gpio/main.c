#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Optputs
#define LED_BOARD   2
#define LED_RED     27
#define LED_GREEN   33

// Filas 1 al 4 (salidas)
#define ROW_1   GPIO_NUM_3
#define ROW_2   GPIO_NUM_21
#define ROW_3   GPIO_NUM_19
#define ROW_4   GPIO_NUM_18

// Columnas 2 al 8  (entradas)
#define COL_1   GPIO_NUM_36
#define COL_2   GPIO_NUM_39
#define COL_3   GPIO_NUM_34
#define COL_4   GPIO_NUM_35

static const char *TAG = "Matrix keyboard";


/**
 * Lectura tradicional utilizando GPIO, sin bibliotecas,
 * haciendo los bucles for.
 */

/**
 * Prototipos de funciones
 */
esp_err_t initInputOutputs(void);
esp_err_t initKeyboardsGpio();


// Estado de LEDS
uint8_t ledBoardState = false;
uint8_t ledRedState = false;
uint8_t ledGreenState = false;


esp_err_t initKeyboardsGpio() {

  // Filas (salidas)
  gpio_config_t io_conf_outputs = {
      .pin_bit_mask = (1ULL << ROW_1) | (1ULL << ROW_2) 
        | (1ULL << ROW_3) | (1ULL << ROW_4),
      .mode = GPIO_MODE_OUTPUT,              // Set as output
      .pull_up_en = GPIO_PULLUP_DISABLE,     // Disable pull-up
      .pull_down_en = GPIO_PULLDOWN_DISABLE, // Disable pull-down
      .intr_type = GPIO_INTR_DISABLE         // Disable interrupts
  };
  gpio_config(&io_conf_outputs);

  // Columnas (entradas)
  gpio_config_t io_conf_inputs = {
      .pin_bit_mask = (1ULL << COL_1) | (1ULL << COL_2) 
        | (1ULL << COL_3) | (1ULL << COL_4),
      .mode = GPIO_MODE_INPUT,                // Set as output
      .pull_up_en = GPIO_PULLUP_DISABLE,       // Disable pull-up
      .pull_down_en = GPIO_PULLDOWN_DISABLE, // Disable pull-down
      .intr_type = GPIO_INTR_DISABLE         // Disable interrupts
  };
  gpio_config(&io_conf_inputs);

  return ESP_OK;
}

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
  // initInputOutputs();
  initKeyboardsGpio();

  gpio_num_t rows[4] = { ROW_1, ROW_2, ROW_3, ROW_4 };
  gpio_num_t columns[4] = { COL_1, COL_2, COL_3, COL_4 };

  // Todos en 1
  gpio_set_level(ROW_1, true);
  gpio_set_level(ROW_2, true);
  gpio_set_level(ROW_3, true);
  gpio_set_level(ROW_4, true);

  char keys[4][4] =   {{'1', '2', '3', 'A'},
                       {'4', '5', '6', 'B'},
                       {'7', '8', '9', 'C'},
                       {'*', '0', '#', 'D'}};


  while (1)
  {
    for (int i = 0; i < 4; i++) // filas
    {
      // ESP_LOGW(TAG, "Row %d", i);
      gpio_set_level(rows[i], false);
      for (int j = 0; j < 4; j++)
      {
        // ESP_LOGW(TAG, "Col %d", j);
        int value = gpio_get_level(columns[j]);
        // ESP_LOGW(TAG, " %d", value);
        if (!value) // columnas
        {
          ESP_LOGE(TAG, "Pressed %c", keys[i][j]);
        }
      }
      gpio_set_level(rows[i], true);
    }
    
    // Delay para permitir que otras tareas (incluyendo IDLE) se ejecuten
    // y evitar que el Task Watchdog reinicie el ESP32
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
