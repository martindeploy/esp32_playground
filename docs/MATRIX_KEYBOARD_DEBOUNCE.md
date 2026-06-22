# Teclado Matricial 4x4 con Anti-rebote para ESP32

Implementación de lectura de teclado matricial 4x4 con **anti-rebote por software** compatible con ESP32 original.

## Características

✅ **Anti-rebote (Debouncing)**: Elimina lecturas falsas por rebotes mecánicos  
✅ **Detección de presión larga**: Eventos especiales para teclas mantenidas  
✅ **Eventos separados**: Press, Release y Long Press  
✅ **Compatible ESP32 original**: No requiere hardware especial  
✅ **Basado en timers FreeRTOS**: Escaneo eficiente sin bloquear el sistema  

## Configuración Hardware

### Conexiones

**Filas (Outputs):**
- ROW_1 → GPIO 3
- ROW_2 → GPIO 21
- ROW_3 → GPIO 19
- ROW_4 → GPIO 18

**Columnas (Inputs con Pull-up):**
- COL_1 → GPIO 36
- COL_2 → GPIO 39
- COL_3 → GPIO 34
- COL_4 → GPIO 35

### Mapa del teclado 4x4

```
    COL_1  COL_2  COL_3  COL_4
    ──────────────────────────
ROW_1│  1     2     3     A
ROW_2│  4     5     6     B
ROW_3│  7     8     9     C
ROW_4│  *     0     #     D
```

## Parámetros Configurables

```c
#define DEBOUNCE_TIME_MS        50    // Tiempo de anti-rebote
#define SCAN_INTERVAL_MS        10    // Frecuencia de escaneo
#define LONG_PRESS_TIME_MS      1000  // Tiempo para presión larga
```

## Uso Básico

```c
void app_main(void)
{
    // Inicializar teclado
    ESP_ERROR_CHECK(keyboard_init());
    
    // Iniciar escaneo
    ESP_ERROR_CHECK(keyboard_start());
    
    // El sistema está listo
    // Los eventos se disparan automáticamente
}
```

## Eventos y Callbacks

### 1. Presión de tecla
```c
static void on_key_pressed(int row, int col, char key_char)
{
    // Se dispara cuando una tecla es presionada
    // Después de pasar el tiempo de anti-rebote
    printf("Tecla presionada: %c\n", key_char);
}
```

### 2. Liberación de tecla
```c
static void on_key_released(int row, int col, char key_char)
{
    // Se dispara cuando una tecla es liberada
    // Después de pasar el tiempo de anti-rebote
    printf("Tecla liberada: %c\n", key_char);
}
```

### 3. Presión larga
```c
static void on_key_long_press(int row, int col, char key_char)
{
    // Se dispara cuando una tecla se mantiene presionada
    // más de LONG_PRESS_TIME_MS
    printf("Presión larga: %c\n", key_char);
}
```

## Cómo Funciona

### 1. Escaneo Matricial
- Un timer ejecuta `scan_keyboard()` cada 10ms
- Por cada fila:
  - Se pone en LOW
  - Se leen todas las columnas
  - Si una columna está en LOW → tecla presionada
  - Se vuelve a poner en HIGH

### 2. Anti-rebote
Cada tecla tiene una máquina de estados:

```
IDLE → PRESSED → (wait debounce) → DEBOUNCED → RELEASED → IDLE
                                    ↓
                                LONG_PRESS (si se mantiene)
```

- **Estado IDLE**: Tecla sin presionar
- **Estado PRESSED**: Detección inicial (puede ser rebote)
- **Estado DEBOUNCED**: Confirmado después de 50ms → dispara `on_key_pressed()`
- **Estado LONG_PRESS**: Si se mantiene 1000ms → dispara `on_key_long_press()`
- **Estado RELEASED**: Liberación confirmada → dispara `on_key_released()`

### 3. Ventajas vs Implementación Simple

| Característica | Simple (07) | Con Anti-rebote (09) |
|----------------|-------------|----------------------|
| Lecturas falsas | ❌ Muchas | ✅ Ninguna |
| Presión larga | ❌ No | ✅ Sí |
| Eventos press/release | ❌ No | ✅ Sí |
| Multi-tecla | ⚠️ Limitado | ✅ Soportado |
| Uso de CPU | 🔴 Alto | 🟢 Bajo |

## Personalización

### Cambiar el mapa de teclas
```c
static const char key_chars[NUM_ROWS][NUM_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
```

### Agregar lógica personalizada
```c
static void on_key_pressed(int row, int col, char key_char)
{
    switch (key_char) {
        case 'A':
            // Ejecutar acción A
            break;
        case '1':
            // Ejecutar acción 1
            break;
        // ... más casos
    }
}
```

### Implementar un buffer de entrada
```c
#define MAX_BUFFER 16
static char input_buffer[MAX_BUFFER];
static int buffer_pos = 0;

static void on_key_pressed(int row, int col, char key_char)
{
    if (buffer_pos < MAX_BUFFER - 1) {
        input_buffer[buffer_pos++] = key_char;
        input_buffer[buffer_pos] = '\0';
        ESP_LOGI(TAG, "Buffer: %s", input_buffer);
    }
    
    if (key_char == '#') {  // Enter
        // Procesar input_buffer
        buffer_pos = 0;
        input_buffer[0] = '\0';
    }
}
```

## Compilación

```bash
# En el CMakeLists.txt de main/, asegúrate de tener:
idf_component_register(SRCS "09_matrix_keyboard_debounce.c" INCLUDE_DIRS ".")

# Compilar
idf.py build

# Flashear
idf.py flash monitor
```

## Troubleshooting

### Problema: Lecturas múltiples de una sola presión
**Solución**: Aumentar `DEBOUNCE_TIME_MS`

### Problema: No detecta presiones rápidas
**Solución**: Reducir `SCAN_INTERVAL_MS`

### Problema: Presión larga se activa muy rápido
**Solución**: Aumentar `LONG_PRESS_TIME_MS`

### Problema: No detecta ninguna tecla
**Verificar**:
- Conexiones físicas correctas
- Pull-ups activados en columnas
- Filas configuradas como outputs

## Comparación con otras soluciones

### vs `matrix_keyboard` (ESP-IDF component)
- ❌ `matrix_keyboard` requiere `dedic_gpio` → No funciona en ESP32 original
- ✅ Esta implementación usa GPIO estándar → Compatible con todos los ESP32

### vs `iot_button` (ESP-IDF component)
- ❌ `iot_button` requiere un GPIO por botón → 16 GPIOs para 16 teclas
- ✅ Teclado matricial usa 8 GPIOs → Más eficiente

### vs Implementación simple con delays
- ❌ Delays bloqueantes → No puede hacer otras tareas
- ✅ Timer-based → Sistema no bloqueante

## Licencia

SPDX-License-Identifier: Apache-2.0
