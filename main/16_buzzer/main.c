#include <stdio.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Optputs
#define BUZZER_GPIO             GPIO_NUM_10
#define BUZZER_LEDC_SPEED_MODE  LEDC_LOW_SPEED_MODE
#define BUZZER_LEDC_CHANNEL     LEDC_CHANNEL_0
#define BUZZER_LEDC_TIMER       LEDC_TIMER_0
#define BUZZER_LEDC_RESOLUTION  LEDC_TIMER_10_BIT // Resolución de 10 bits 

static const char *TAG = "16_buzzer";


/**
 * Prototipos de funciones
 */
esp_err_t init_buzzer_pwm(void);
void play_tone(uint32_t frequency_hz, uint32_t duration_ms);


void app_main(void)
{
  init_buzzer_pwm();
  while (1) {

    // Reproducir tono de 1000 Hz por 1000 ms
    play_tone(1000, 1000);

    // Reproducir tono de 2000 Hz por 1000 ms
    play_tone(2000, 1000);

    // Reproducir tono de 2000 Hz por 1000 ms
    play_tone(3000, 1000);

    // Silencio por 2 segundo
    play_tone(0, 2000);
  }
}

esp_err_t init_buzzer_pwm(void)
{
  // 1. Configurar el temporizador LEDC
  ledc_timer_config_t ledc_timer = {
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .timer_num = BUZZER_LEDC_TIMER,
      .duty_resolution = BUZZER_LEDC_RESOLUTION,
      .freq_hz = 1000, // Frecuencia inicial en Hz
      .clk_cfg = LEDC_AUTO_CLK};
  ledc_timer_config(&ledc_timer);

  // 2. Configurar el canal LEDC
  ledc_channel_config_t ledc_channel = {
      .speed_mode = BUZZER_LEDC_SPEED_MODE,
      .channel = BUZZER_LEDC_CHANNEL,
      .timer_sel = BUZZER_LEDC_TIMER,
      .intr_type = LEDC_INTR_DISABLE,
      .gpio_num = BUZZER_GPIO,
      .duty = 0, // Apagado inicialmente
      .hpoint = 0};
  ledc_channel_config(&ledc_channel);

  return ESP_OK;
}

// Función para hacer sonar el buzzer a una frecuencia específica
void play_tone(uint32_t frequency_hz, uint32_t duration_ms)
{
  if (frequency_hz == 0)
  {
    // Detener el sonido (Duty en 0)
    ledc_set_duty(BUZZER_LEDC_SPEED_MODE, BUZZER_LEDC_CHANNEL, 0);
    ledc_update_duty(BUZZER_LEDC_SPEED_MODE, BUZZER_LEDC_CHANNEL);
  }
  else
  {
    // Cambiar la frecuencia del temporizador
    ledc_set_freq(BUZZER_LEDC_SPEED_MODE, BUZZER_LEDC_TIMER, frequency_hz);

    // Configurar ciclo de trabajo al 50% (para ondas cuadradas óptimas en un buzzer)
    // 50% de una resolución de 10 bits (1024) es 512
    ledc_set_duty(BUZZER_LEDC_SPEED_MODE, BUZZER_LEDC_CHANNEL, 512);
    ledc_update_duty(BUZZER_LEDC_SPEED_MODE, BUZZER_LEDC_CHANNEL);
  }

  // Esperar la duración del tono
  vTaskDelay(pdMS_TO_TICKS(duration_ms));
}
