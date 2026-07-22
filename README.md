## ESP 32 playground

- **01_blink_blocking**: Blink básico con `vTaskDelay` bloqueante y logging por UART.
- **02_blink_timers**: Blink con ESP Timer (no bloqueante), manejo de 2 LEDs con frecuencias independientes.
- **03_spi_matrix**: Control de matriz LED 8x8 con MAX7219 via SPI. [Ver video](https://www.youtube.com/watch?v=OiKF7yPsxXM&t=11s)
- **04_interrupts**: Lectura de pulsador con interrupción por flanco, pull-up externo y capacitor anti-rebote.
- **05_ble_innovate_yourself**: Servidor GATT BLE básico. [Ver video](https://www.youtube.com/watch?v=EIo5aZ3c89Q)
- **06_lcd_gpio**: Control de pantalla LCD 128x64 con ST7920 via GPIO (sin driver SPI).
- **07_matrix_keyboard_gpio**: Lectura de teclado matricial 4x4 con GPIO puro, sin bibliotecas externas.
- **08_matrix_keyboard_idf_sample**: Lectura de teclado matricial usando la biblioteca `matrix_keyboard` de ESP-IDF. Solo compatible con ESP32-S3 (requiere `dedic_gpio`).
- **09_matrix_keyboard_debounce**: Lectura de teclado matricial 4x4 con debouncing por software.
- **10_ble_beacon**: Advertising BLE sin posibilidad de conexión (beacon). Emite paquetes GAP periódicamente. Basado en ejemplo ESP-IDF.
- **11_ble_connection**: BLE connectable con GAP. Permite establecer conexión desde un cliente. Enciende el LED al conectarse y apaga al desconectarse. Basado en ejemplo ESP-IDF.
- **12_ble_gatt_server**: Servidor GATT BLE con characteristics de lectura/escritura y notify para frecuencia cardíaca (heart rate). Controla un LED elegido. Basado en ejemplo ESP-IDF.
- **13_ble_hid**: Dispositivo HID BLE. Soporta mouse, teclado y gamepad según configuración.
- **14_lcd_i2c**: Control de LCD 16x2 en modo texto via I2C con ESP32-C3.
- **15_joystick**: Lectura analógica de joystick HW-504 (ejes X/Y) con ADC oneshot y botón por GPIO.
- **16_buzzer**: Control de buzzer pasivo HW-508 con PWM via LEDC, generación de tonos por frecuencia.
- **17_tiny_rtc**: Lectura y escritura de fecha/hora con módulo Tiny RTC (DS1307) via I2C usando la biblioteca `esp-idf-lib`.
- **18_spi_matrix_lib**: Control de matriz LED 8x8 con MAX7219 via SPI usando la biblioteca `esp-idf-lib` (max7219), con símbolos, dígitos y animaciones de scroll.
- **19_object_pattern**: Patrón orientado a objetos en C con struct `led_controller` que encapsula estado, inicialización, control de encendido/apagado y blink no bloqueante por GPIO.
- **20_http_server**: Servidor HTTP embebido en ESP32 funcionando como Access Point. [Workshop Espressif](https://developer.espressif.com/workshops/esp-idf-basic/assignment-2-1/)



> Importante: Para que funcione BLE y compile correctamente debe estar habilitado desde sdkconfig los siguientes parametros:

```
#
# Bluetooth
#
CONFIG_BT_ENABLED=y
# CONFIG_BT_BLUEDROID_ENABLED is not set
CONFIG_BT_NIMBLE_ENABLED=y
# CONFIG_BT_CONTROLLER_ONLY is not set
# default:
CONFIG_BT_CONTROLLER_ENABLED=y
# default:
# CONFIG_BT_CONTROLLER_DISABLED is not set

# NimBLE Options
#
CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_INTERNAL=y
# CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_DEFAULT is not set
# CONFIG_BT_NIMBLE_LOG_LEVEL_NONE is not set
# CONFIG_BT_NIMBLE_LOG_LEVEL_ERROR is not set
# CONFIG_BT_NIMBLE_LOG_LEVEL_WARNING is not set
CONFIG_BT_NIMBLE_LOG_LEVEL_INFO=y
# CONFIG_BT_NIMBLE_LOG_LEVEL_DEBUG is not set
CONFIG_BT_NIMBLE_LOG_LEVEL=1
CONFIG_BT_NIMBLE_MAX_CONNECTIONS=3
CONFIG_BT_NIMBLE_MAX_BONDS=3
CONFIG_BT_NIMBLE_MAX_CCCDS=8
CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM=0
CONFIG_BT_NIMBLE_PINNED_TO_CORE_0=y
# CONFIG_BT_NIMBLE_PINNED_TO_CORE_1 is not set
CONFIG_BT_NIMBLE_PINNED_TO_CORE=0
CONFIG_BT_NIMBLE_HOST_TASK_STACK_SIZE=4096
CONFIG_BT_NIMBLE_ROLE_CENTRAL=y
CONFIG_BT_NIMBLE_ROLE_PERIPHERAL=y
CONFIG_BT_NIMBLE_ROLE_BROADCASTER=y
CONFIG_BT_NIMBLE_ROLE_OBSERVER=y
# CONFIG_BT_NIMBLE_NVS_PERSIST is not set
CONFIG_BT_NIMBLE_SECURITY_ENABLE=y
CONFIG_BT_NIMBLE_SM_LEGACY=y
CONFIG_BT_NIMBLE_SM_SC=ys
# CONFIG_BT_NIMBLE_SM_SC_DEBUG_KEYS is not set
CONFIG_BT_NIMBLE_LL_CFG_FEAT_LE_ENCRYPTION=y
# CONFIG_BT_NIMBLE_DEBUG is not set
CONFIG_BT_NIMBLE_SVC_GAP_DEVICE_NAME="nimble"
CONFIG_BT_NIMBLE_GAP_DEVICE_NAME_MAX_LEN=31
CONFIG_BT_NIMBLE_ATT_PREFERRED_MTU=256
CONFIG_BT_NIMBLE_SVC_GAP_APPEARANCE=0
```

- 06_lcd_gpio.c: Control de LCD en modo texto sin bibliotecas externas y un poco de ayuda de Cloude.
    
    
    Links útiles: [Conexión con Python](https://medium.com/@marcj_40686/raspberry-pi-and-st7920-lcd-displaying-text-and-more-51e2c7eb04df)



## Anotaciones SPI

En ESP32 SPI1 y SPI2 son general purpose.

- MOSI: Master Out Slave In.
- MISO: Master In Slave Out.
- SCLK: Serial Clock.
- CS: Chip Select.

Transaction: Atomica no se puede interrumpir.
    - Por interrupciones.
    - Por polling.


Ver docs/spi

[Playlist YouTube](https://www.youtube.com/playlist?list=PL-Hb9zZP9qC65SpXHnTAO0-qV6x5JxCMJ)