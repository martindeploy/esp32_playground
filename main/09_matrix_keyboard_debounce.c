#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

static const char *TAG = "matrix_keyboard_debounce";

// Filas (salidas)
#define ROW_1   GPIO_NUM_3
#define ROW_2   GPIO_NUM_21
#define ROW_3   GPIO_NUM_19
#define ROW_4   GPIO_NUM_18

// Columnas (entradas)
#define COL_1   GPIO_NUM_36
#define COL_2   GPIO_NUM_39
#define COL_3   GPIO_NUM_34
#define COL_4   GPIO_NUM_35

// Configuración de anti-rebote
#define DEBOUNCE_TIME_MS        50    // Tiempo de anti-rebote en ms
#define SCAN_INTERVAL_MS        10    // Intervalo de escaneo
#define LONG_PRESS_TIME_MS      1000  // Tiempo para considerar presión larga

// Dimensiones del teclado
#define NUM_ROWS    4
#define NUM_COLS    4

// Estados de las teclas
typedef enum {
    KEY_STATE_IDLE = 0,
    KEY_STATE_PRESSED,
    KEY_STATE_RELEASED,
    KEY_STATE_LONG_PRESS
} key_state_t;

// Estructura para cada tecla
typedef struct {
    key_state_t state;
    uint32_t press_time;
    uint32_t last_change_time;
    bool debounced;
    bool long_press_triggered;
} key_info_t;

// Matriz de información de teclas
static key_info_t key_matrix[NUM_ROWS][NUM_COLS];

// Mapa de caracteres del teclado 4x4
static const char key_chars[NUM_ROWS][NUM_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Arrays de pines
static const gpio_num_t rows[NUM_ROWS] = {ROW_1, ROW_2, ROW_3, ROW_4};
static const gpio_num_t cols[NUM_COLS] = {COL_1, COL_2, COL_3, COL_4};

// Timer para escaneo
static TimerHandle_t scan_timer = NULL;

/**
 * @brief Inicializa los GPIOs del teclado matricial
 */
static esp_err_t init_keyboard_gpio(void)
{
    // Configurar filas como salidas
    gpio_config_t io_conf_rows = {
        .pin_bit_mask = (1ULL << ROW_1) | (1ULL << ROW_2) | 
                        (1ULL << ROW_3) | (1ULL << ROW_4),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf_rows));

    // Configurar columnas como entradas con pull-up
    gpio_config_t io_conf_cols = {
        .pin_bit_mask = (1ULL << COL_1) | (1ULL << COL_2) | 
                        (1ULL << COL_3) | (1ULL << COL_4),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,  // resisitencias Pull-Up de 1k en protoboard a 3.3V
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf_cols));

    // Inicializar todas las filas en HIGH
    for (int i = 0; i < NUM_ROWS; i++) {
        gpio_set_level(rows[i], 1);
    }

    ESP_LOGI(TAG, "GPIO inicializado correctamente");
    return ESP_OK;
}

/**
 * @brief Callback para evento de tecla presionada
 */
static void on_key_pressed(int row, int col, char key_char)
{
    ESP_LOGI(TAG, "🔵 TECLA PRESIONADA: '%c' [Fila:%d, Col:%d]", key_char, row, col);
    // Aquí puedes agregar tu lógica personalizada
}

/**
 * @brief Callback para evento de tecla liberada
 */
static void on_key_released(int row, int col, char key_char)
{
    ESP_LOGI(TAG, "⚪ TECLA LIBERADA: '%c' [Fila:%d, Col:%d]", key_char, row, col);
    // Aquí puedes agregar tu lógica personalizada
}

/**
 * @brief Callback para evento de presión larga
 */
static void on_key_long_press(int row, int col, char key_char)
{
    ESP_LOGI(TAG, "🔴 PRESIÓN LARGA: '%c' [Fila:%d, Col:%d]", key_char, row, col);
    // Aquí puedes agregar tu lógica personalizada
}

/**
 * @brief Escanea el teclado matricial con anti-rebote
 */
static void scan_keyboard(void)
{
    uint32_t current_time = xTaskGetTickCount() * portTICK_PERIOD_MS;

    for (int row = 0; row < NUM_ROWS; row++) {
        // Activar fila actual (LOW)
        gpio_set_level(rows[row], 0);
        
        // Pequeño delay para estabilización
        esp_rom_delay_us(10);

        // Leer todas las columnas
        for (int col = 0; col < NUM_COLS; col++) {
            bool is_pressed = (gpio_get_level(cols[col]) == 0);  // LOW = presionado
            key_info_t *key = &key_matrix[row][col];
            char key_char = key_chars[row][col];

            if (is_pressed) {
                // Tecla está físicamente presionada
                if (key->state == KEY_STATE_IDLE) {
                    // Primera detección de presión
                    key->state = KEY_STATE_PRESSED;
                    key->press_time = current_time;
                    key->last_change_time = current_time;
                    key->debounced = false;
                    key->long_press_triggered = false;
                } 
                else if (key->state == KEY_STATE_PRESSED && !key->debounced) {
                    // Verificar si pasó el tiempo de anti-rebote
                    if ((current_time - key->last_change_time) >= DEBOUNCE_TIME_MS) {
                        key->debounced = true;
                        on_key_pressed(row, col, key_char);
                    }
                }
                else if (key->debounced && !key->long_press_triggered) {
                    // Verificar presión larga
                    if ((current_time - key->press_time) >= LONG_PRESS_TIME_MS) {
                        key->long_press_triggered = true;
                        on_key_long_press(row, col, key_char);
                    }
                }
            } 
            else {
                // Tecla está físicamente liberada
                if (key->state == KEY_STATE_PRESSED && key->debounced) {
                    // Cambio de estado: presionada -> liberada
                    key->state = KEY_STATE_RELEASED;
                    key->last_change_time = current_time;
                } 
                else if (key->state == KEY_STATE_RELEASED) {
                    // Verificar anti-rebote en la liberación
                    if ((current_time - key->last_change_time) >= DEBOUNCE_TIME_MS) {
                        on_key_released(row, col, key_char);
                        key->state = KEY_STATE_IDLE;
                        key->debounced = false;
                    }
                }
                else if (key->state == KEY_STATE_PRESSED && !key->debounced) {
                    // Liberación antes de confirmar presión (rebote)
                    key->state = KEY_STATE_IDLE;
                }
            }
        }

        // Desactivar fila (HIGH)
        gpio_set_level(rows[row], 1);
    }
}

/**
 * @brief Callback del timer de escaneo
 */
static void scan_timer_callback(TimerHandle_t xTimer)
{
    scan_keyboard();
}

/**
 * @brief Inicializa el sistema de teclado con anti-rebote
 */
esp_err_t keyboard_init(void)
{
    // Inicializar matriz de estados
    memset(key_matrix, 0, sizeof(key_matrix));

    // Inicializar GPIOs
    ESP_ERROR_CHECK(init_keyboard_gpio());

    // Crear timer periódico para escaneo
    scan_timer = xTimerCreate(
        "kbd_scan",
        pdMS_TO_TICKS(SCAN_INTERVAL_MS),
        pdTRUE,  // Auto-reload
        NULL,
        scan_timer_callback
    );

    if (scan_timer == NULL) {
        ESP_LOGE(TAG, "Error al crear timer de escaneo");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Teclado matricial inicializado");
    ESP_LOGI(TAG, "- Tiempo de anti-rebote: %d ms", DEBOUNCE_TIME_MS);
    ESP_LOGI(TAG, "- Tiempo presión larga: %d ms", LONG_PRESS_TIME_MS);
    ESP_LOGI(TAG, "- Intervalo de escaneo: %d ms", SCAN_INTERVAL_MS);

    return ESP_OK;
}

/**
 * @brief Inicia el escaneo del teclado
 */
esp_err_t keyboard_start(void)
{
    if (scan_timer == NULL) {
        ESP_LOGE(TAG, "Timer no inicializado");
        return ESP_FAIL;
    }

    if (xTimerStart(scan_timer, 0) != pdPASS) {
        ESP_LOGE(TAG, "Error al iniciar timer");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Escaneo de teclado iniciado");
    return ESP_OK;
}

/**
 * @brief Detiene el escaneo del teclado
 */
esp_err_t keyboard_stop(void)
{
    if (scan_timer == NULL) {
        return ESP_FAIL;
    }

    xTimerStop(scan_timer, 0);
    ESP_LOGI(TAG, "Escaneo de teclado detenido");
    return ESP_OK;
}

void app_main(void)
{
    ESP_LOGI(TAG, "=== Teclado Matricial 4x4 con Anti-rebote ===");
    
    // Inicializar teclado
    ESP_ERROR_CHECK(keyboard_init());
    
    // Iniciar escaneo
    ESP_ERROR_CHECK(keyboard_start());
    
    ESP_LOGI(TAG, "Sistema listo. Presiona teclas en el teclado 4x4");
    
    // El resto del programa continúa
    while (1) {
        // Aquí puedes ejecutar otras tareas
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
