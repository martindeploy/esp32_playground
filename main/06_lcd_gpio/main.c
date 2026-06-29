#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Optputs
#define LED_BOARD   2
#define LED_RED     27
#define LED_GREEN   33



// Pines del LCD
#define PIN_SCLK GPIO_NUM_22 // E
#define PIN_SID  GPIO_NUM_23 // R/W
#define PIN_CS   GPIO_NUM_5  // RS
#define PIN_RST  GPIO_NUM_4  // RST
// PSB -> GND


// ST7920 Comandos
#define ST7920_FUNCTION_SET     0x30  // 8-bit mode
#define ST7920_FUNCTION_SET_EXT 0x34  // Extended instruction
#define ST7920_DISPLAY_ON       0x0C  // Display on
#define ST7920_CLEAR_DISPLAY    0x01  // Clear
#define ST7920_HOME             0x02  // Cursor home
#define ST7920_ENTRY_MODE       0x06  // Entry mode


static const char *TAG = "SPI LCD por GPIO";


/**
 * Prototipos de funciones
 */
static inline void delay_us(uint32_t us);
static void gpio_init_output(gpio_num_t pin);
static void send_bit(uint8_t bit);
static void send_byte(uint8_t byte);
static void st7920_write(uint8_t data, uint8_t is_data);
static void st7920_cmd(uint8_t cmd);

// Estado de LEDS
uint8_t ledBoardState = false;
uint8_t ledRedState = false;
uint8_t ledGreenState = false;


// ============================================================================
// PASO 1: Funciones básicas de GPIO
// ============================================================================
static inline void delay_us(uint32_t us) {
    esp_rom_delay_us(us);
}

static void gpio_init_output(gpio_num_t pin) {
    gpio_config_t io = {
        .pin_bit_mask = 1ULL << pin,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io);
}

// ============================================================================
// PASO 2: Envío de un bit por software SPI
// ============================================================================
/**
 * Envía un bit por el pin SID y genera un pulso de reloj en SCLK.
 * El ST7920 lee en el flanco de subida del reloj.
 */
static void send_bit(uint8_t bit) {
    gpio_set_level(PIN_SID, bit ? 1 : 0);   // Poner el bit en SID
    delay_us(1);                            // Esperar setup time
    gpio_set_level(PIN_SCLK, 1);            // Flanco de subida
    delay_us(1);                            // Hold time
    gpio_set_level(PIN_SCLK, 0);            // Flanco de bajada
    delay_us(1);
}
/**
 * Envía un byte completo (8 bits) bit a bit, MSB primero.
 */
static void send_byte(uint8_t byte) {
    for (int i = 7; i >= 0; i--) {
        send_bit((byte >> i) & 0x01);
    }
}

// ============================================================================
// PASO 3: Protocolo de 3 bytes del ST7920
// ============================================================================
/**
 * Envía un comando o dato al ST7920.
 * 
 * @param data El byte de comando/dato a enviar
 * @param is_data 0=comando, 1=dato
 * 
 * Secuencia: CS=1, enviar 3 bytes, CS=0, esperar 72us
 */
static void st7920_write(uint8_t data, uint8_t is_data) {
    gpio_set_level(PIN_CS, 1);  // Activar chip
    delay_us(1);
    
    // Byte 1: Sync byte (0xF8=comando, 0xFA=dato)
    send_byte(is_data ? 0xFA : 0xF8);
    
    // Byte 2: Nibble alto
    send_byte(data & 0xF0);
    
    // Byte 3: Nibble bajo
    send_byte((data << 4) & 0xF0);
    
    gpio_set_level(PIN_CS, 0);  // Desactivar chip
    delay_us(72);  // El ST7920 necesita tiempo para procesar (min 72us)
}


// Wrappers para claridad
static void st7920_cmd(uint8_t cmd) {
    st7920_write(cmd, 0);
}

static void st7920_data(uint8_t data) {
    st7920_write(data, 1);
}


// ============================================================================
// PASO 4: Inicialización del display
// ============================================================================
/**
 * Inicializa el ST7920 en modo texto básico.
 * Sigue la secuencia del datasheet.
 */
static void st7920_init(void) {
    // Configurar pines
    gpio_init_output(PIN_SCLK);
    gpio_init_output(PIN_SID);
    gpio_init_output(PIN_CS);
    gpio_init_output(PIN_RST);
    
    // Estado inicial
    gpio_set_level(PIN_CS, 0);
    gpio_set_level(PIN_SCLK, 0);
    gpio_set_level(PIN_SID, 0);
    
    // Reset por hardware
    gpio_set_level(PIN_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(PIN_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(50));  // Esperar power-up (40ms mínimo)
    
    ESP_LOGI(TAG, "Iniciando secuencia de comandos...");
    
    // Secuencia de inicialización del datasheet
    st7920_cmd(ST7920_FUNCTION_SET);  // Basic instruction set (8-bit interface)
    delay_us(100);
    st7920_cmd(ST7920_FUNCTION_SET);  // Repetir (recomendado por datasheet)
    delay_us(37);
    st7920_cmd(ST7920_DISPLAY_ON);  // Display ON, cursor OFF
    delay_us(100);
    st7920_cmd(ST7920_CLEAR_DISPLAY);  // Clear display
    vTaskDelay(pdMS_TO_TICKS(10));  // Clear necesita 10ms
    st7920_cmd(ST7920_ENTRY_MODE);  // Entry mode: cursor moves right
    delay_us(100);
    
    ESP_LOGI(TAG, "ST7920 inicializado en modo texto");
}


// ============================================================================
// PASO 5: Funciones de texto básico
// ============================================================================
/**
 * Posiciona el cursor en modo texto.
 * 
 * @param row Fila (0-3)
 * @param col Columna (0-15 para líneas 0,1; 0-7 para líneas 2,3)
 * 
 * Nota: El ST7920 tiene un mapeo extraño de la DDRAM:
 * - Línea 0: 0x80 - 0x8F
 * - Línea 1: 0x90 - 0x9F
 * - Línea 2: 0x88 - 0x8F
 * - Línea 3: 0x98 - 0x9F
 */
static void st7920_set_cursor(uint8_t row, uint8_t col) {
    uint8_t addr;
    
    switch(row) {
        case 0: addr = 0x80 + col; break;
        case 1: addr = 0x90 + col; break;
        case 2: addr = 0x88 + col; break;
        case 3: addr = 0x98 + col; break;
        default: addr = 0x80; break;
    }
    
    st7920_cmd(addr);
}

/**
 * Escribe una cadena en la posición actual del cursor.
 */
static void st7920_print(const char *str) {
    while (*str) {
        st7920_data(*str);
        str++;
        delay_us(50);  // Pequeña pausa entre caracteres
    }
}


void app_main(void) {
    ESP_LOGI(TAG, "ST7920 Raw Driver Demo");
    ESP_LOGI(TAG, "Sin bibliotecas externas - Control total");
    
    st7920_init();
    
    // Escribir texto en las 4 líneas (16 chars)
    st7920_set_cursor(0, 0);
    // st7920_print("Hola ST7920!");
    st7920_print("> Martin Pablo <");

    
    st7920_set_cursor(1, 0);
    st7920_print("Driver desde 0");
    
    st7920_set_cursor(2, 0);
    st7920_print("Sin u8g2 :)");
    
    st7920_set_cursor(3, 0);
    st7920_print("By martindeploy");
    
    ESP_LOGI(TAG, "Texto mostrado. Parpadeo cada 2 segundos...");
    
    // Loop simple: parpadear el display
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(2000));
        st7920_cmd(0x08);  // Display OFF
        
        vTaskDelay(pdMS_TO_TICKS(500));
        st7920_cmd(0x0C);  // Display ON
    }
}