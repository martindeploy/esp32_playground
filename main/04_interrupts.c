#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "04_interrupts";

// Optputs
#define LED_BOARD   2
#define LED_RED     25
#define LED_GREEN   33

// Inputs
#define PUSH_01     27 // con resistencia pull up y capacitor 22pf 



/**
 * Prototipos de funciones
 */
esp_err_t initInputOutputs(void);
esp_err_t initIrs(void);
void isrHandler(void *args);
void blink_led(uint8_t *ledLevel);
void updateLedsState();


// Estado de LEDS
uint8_t ledBoardState = false;
uint8_t ledRedState = false;
uint8_t ledGreenState = false;

uint8_t count = 0;


void app_main(void)
{
  ESP_ERROR_CHECK(initInputOutputs());
  ESP_ERROR_CHECK(initIrs());

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


/**
 * Inicializar botón/interruptor
 */
esp_err_t initIrs(void) {

  ESP_LOGI(TAG, "Starting init IRQ Button - GPIO %d", PUSH_01);

  gpio_config_t pGPIOConfig;
  pGPIOConfig.pin_bit_mask = (1ULL << PUSH_01);
  pGPIOConfig.mode = GPIO_MODE_DEF_INPUT;
  pGPIOConfig.pull_up_en = GPIO_PULLUP_DISABLE;
  pGPIOConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
  pGPIOConfig.intr_type = GPIO_INTR_NEGEDGE;
  
  gpio_config(&pGPIOConfig);

  ESP_LOGI(TAG, "  - Pin: GPIO %d", PUSH_01);
  ESP_LOGI(TAG, "  - Mode: INPUT");
  ESP_LOGI(TAG, "  - Interrupt: NEGEDGE");

  gpio_install_isr_service(0);
  gpio_isr_handler_add(PUSH_01, isrHandler, NULL );

  ESP_LOGI(TAG, "Init IRQ Button completed\n");

  return ESP_OK;
}

/**
 * Callback de la interrupcion. No puede llevar nada bloqueante. La llama la interrupcion.
 * como por ejemplo un ESP_LOG
 */
void isrHandler(void *args) {

  // TODO: Investigar que se puede recuperar de *args.
  
  count ++;
  if(count > 2){
    count = 0;
  }

  switch (count)
  {
  case 0:
    gpio_set_level(LED_RED, 1);
    gpio_set_level(LED_GREEN, 0);
    gpio_set_level(LED_BOARD, 0);
    break;
  case 1:
    gpio_set_level(LED_RED, 0);
    gpio_set_level(LED_GREEN, 1);
    gpio_set_level(LED_BOARD, 0);
    break;
  case 2:
    gpio_set_level(LED_RED, 0);
    gpio_set_level(LED_GREEN, 0);
    gpio_set_level(LED_BOARD, 1);
    break;
  default:
    break;
  }

}

/**
 * Blink
 */
void blink_led(uint8_t *ledLevel)
{
  ESP_LOGI(TAG, "Invocation blink_led");
  *ledLevel = !*ledLevel;
  updateLedsState();
}

/**
 * Actualiza cada una de las salidas
 */
void updateLedsState()
{
  gpio_set_level(LED_BOARD, ledBoardState);
  gpio_set_level(LED_GREEN, ledGreenState);
  gpio_set_level(LED_RED, ledRedState);
}