## ESP 32 playground

- 01_blink_blocking.c: Ejemplo blink con delay bloqueantes y log.
- 02_blink_timers.c: Ejemplo blink con timer, posibilidad de enceder y apagar 2 LEDS con distinto tiempo.
- 03_spi_matrix.c: Prueba de matriz LED 8x8 con MAX7219. Ver datasheets en docs. [Ver video](https://www.youtube.com/watch?v=OiKF7yPsxXM&t=11s)
- 04_interrupts.c: Interrupciones por hardware utilizando un push button, resistencia pull up y capacitor de 33pF.
- 05_ble_connect.c: Servidor Gatt por BLE. [Ver video](https://www.youtube.com/watch?v=EIo5aZ3c89Q)
- 06_lcd_gpio.c: Conexión de pantalla LCD 4 líneas con ST7920 por GPIO (sin config SPI)
- 07_matrix_keyboard_gpio.c: Lectura de teclado matricial 4x4 tradicional utilizando GPIO, sin bibliotecas.
- 08_matrix_keyboard_library.c: Lectura de teclado matricial 4x4 ejemplo ESP-IDF solo válido para ESP32-S3.
- 09_matrix_keyboard_debounce.c: Lectura de teclado matricial 4x4 con debouncing.
- 10_ble_beacon.c:
- 11_ble_beacon.c:
- 12_ble_beacon.c:



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