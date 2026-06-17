#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

// Optputs
#define LED_BOARD   2
#define LED_RED     27
#define LED_GREEN   33

// Tiempo en ms
#define INTERVAL_TIMER_ONE    500   // Board
#define INTERVAL_TIMER_TWO    300   // Green
#define INTERVAL_TIMER_THREE  10000  // Red


static const char *TAG = "02_blink_timers";


/**
 * Prototipos de funciones
 */
esp_err_t init_leds(void);
esp_err_t initializeTimers(void);
void vTimerOneCallback(TimerHandle_t xTimer); // Funcion de callback que invoca el timer
void vTimerTwoCallback(TimerHandle_t xTimer); // Funcion de callback que invoca el timer
void vTimerThreeCallback(TimerHandle_t xTimer); // Funcion de callback que invoca el timer
esp_err_t blink_led(uint8_t* state);
esp_err_t updateLedsState();




// Led board 500 ms
// Timer one auto reload (comienza nuevamente)
TimerHandle_t xTimerOne;
int intervalOne = INTERVAL_TIMER_ONE;
int timerOneId = 1;


// LED Red
// Timer one auto reload (comienza nuevamente)
TimerHandle_t xTimerTwo;
int intervalTwo = INTERVAL_TIMER_TWO;
int timerTwoId = 2;


// LED Green
// Timer three auto reload (sin auto reload)
TimerHandle_t xTimerThree;
int intervalThree = INTERVAL_TIMER_THREE;
int timerThreeeId = 3;


// Estado de LEDS
uint8_t ledBoardState = false;
uint8_t ledRedState = false;
uint8_t ledGreenState = false;


/**
 * Inicializar I/O
 */
esp_err_t init_leds(void)
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


/*
* Callback function, al finalizar el timer
*/
void vTimerOneCallback(TimerHandle_t xTimer)
{
  ESP_LOGI(TAG, "Event was called from timer vTimerTwoCallback");
  
  // Recuperamos el ID del temporizador si es necesario
  uint32_t timer_id = (uint32_t)pvTimerGetTimerID(xTimer);
  ESP_LOGI(TAG, "¡Temporizador %lu ha expirado!", timer_id);

  blink_led(&ledBoardState);
}

/*
* Callback function, al finalizar el timer
*/
void vTimerTwoCallback(TimerHandle_t xTimer)
{
  ESP_LOGI(TAG, "Event was called from timer vTimerTwoCallback");
  
  // Recuperamos el ID del temporizador si es necesario
  uint32_t timer_id = (uint32_t)pvTimerGetTimerID(xTimer);
  ESP_LOGI(TAG, "¡Temporizador %lu ha expirado!", timer_id);

  blink_led(&ledGreenState);
}

/*
* Callback function, al finalizar el timer
*/
void vTimerThreeCallback(TimerHandle_t xTimer)
{
  ESP_LOGI(TAG, "Event was called from timer vTimerThreeCallback");
  
  // Recuperamos el ID del temporizador si es necesario
  uint32_t timer_id = (uint32_t)pvTimerGetTimerID(xTimer);
  ESP_LOGE(TAG, "¡Temporizador %lu ha expirado y no se vuelve a iniciar!", timer_id);

  blink_led(&ledRedState);
}


/**
 *  Inicializa un timer con un intervalo, recibe un callback
 */
esp_err_t initializeTimers(void)
{
  ESP_LOGI(TAG, "Timers init configuration");

  // Timer One
  xTimerOne = xTimerCreate("MyTimerOne",                 // Just a text name, not used by the kernel.
                        pdMS_TO_TICKS(intervalOne), // The timer period in ticks.
                        pdTRUE,           // The timers will auto-reload themselves when they expire. pdTRUE: periodico
                        (void *)timerOneId,         // Assign each timer a unique id equal to its array index.
                        vTimerOneCallback          // Each timer calls the same callback when it expires.
  );
  if (xTimerOne == NULL)
  {
    // The timer was not created.
    ESP_LOGE(TAG, "The timer was not created");
  }
  else
  {
    // Start the timer.  No block time is specified, and even if one was
    // it would be ignored because the scheduler has not yet been
    // started.
    if (xTimerStart(xTimerOne, 0) != pdPASS)
    {
      // The timer could not be set into the Active state.
      ESP_LOGE(TAG, "The timer could not be set into the Active state.");
    }
  }

  // Timer Two
  xTimerTwo = xTimerCreate("MyTimerTwo",                 // Just a text name, not used by the kernel.
                        pdMS_TO_TICKS(intervalTwo), // The timer period in ticks.
                        pdTRUE,           // The timers will auto-reload themselves when they expire. pdTRUE: periodico
                        (void *)timerTwoId,         // Assign each timer a unique id equal to its array index.
                        vTimerTwoCallback          // Each timer calls the same callback when it expires.
  );
  if (xTimerTwo == NULL)
  {
    // The timer was not created.
    ESP_LOGE(TAG, "The timer was not created");
  }
  else
  {
    // Start the timer.  No block time is specified, and even if one was
    // it would be ignored because the scheduler has not yet been
    // started.
    if (xTimerStart(xTimerTwo, 0) != pdPASS)
    {
      // The timer could not be set into the Active state.
      ESP_LOGE(TAG, "The timer could not be set into the Active state.");
    }
  }

  // Timer three
  xTimerThree = xTimerCreate("MyTimerThree",                 // Just a text name, not used by the kernel.
                        pdMS_TO_TICKS(intervalThree), // The timer period in ticks.
                        pdFALSE,           // The timers will auto-reload themselves when they expire. pdTRUE: periodico
                        (void *)timerThreeeId,         // Assign each timer a unique id equal to its array index.
                        vTimerThreeCallback          // Each timer calls the same callback when it expires.
  );
  if (xTimerThree == NULL)
  {
    // The timer was not created.
    ESP_LOGE(TAG, "The timer was not created");
  }
  else
  {
    // Start the timer.  No block time is specified, and even if one was
    // it would be ignored because the scheduler has not yet been
    // started.
    if (xTimerStart(xTimerThree, 0) != pdPASS)
    {
      // The timer could not be set into the Active state.
      ESP_LOGE(TAG, "The timer could not be set into the Active state.");
    }
  }

  return ESP_OK;
}



/**
 * Blink
 */
esp_err_t blink_led(uint8_t *ledLevel)
{
  ESP_LOGI(TAG, "Invocation blink_led");
  *ledLevel = !*ledLevel;
  updateLedsState();
  return ESP_OK;
}

/**
 * Actualiza cada una de las salidas
 */
esp_err_t updateLedsState()
{
  gpio_set_level(LED_BOARD, ledBoardState);
  gpio_set_level(LED_GREEN, ledGreenState);
  gpio_set_level(LED_RED, ledRedState);

  return ESP_OK;
}

void app_main(void)
{
  init_leds();
  initializeTimers();
  
}
