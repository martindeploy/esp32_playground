# Delays bloqueantes similar Arduino
# idf_component_register(SRCS "01_blink_blocking.c" INCLUDE_DIRS ".")

# Delays con timer
# idf_component_register(SRCS "02_blink_timers.c" INCLUDE_DIRS ".")

# SPI Matrix LED
# idf_component_register(SRCS "03_spi_matrix.c" INCLUDE_DIRS ".")

# Interrupciones y pulsadores
# idf_component_register(SRCS "04_interrupts.c" INCLUDE_DIRS ".")

# Bluetooth BLE
# idf_component_register(SRCS "05_ble_connect.c" INCLUDE_DIRS ".")

# Control LCD por GPIO (sin config SPI)
# idf_component_register(SRCS "06_lcd_gpio.c" INCLUDE_DIRS ".")

# Lectura de teclado matricial 4x4 utilizando GPIO y bucles for
# idf_component_register(SRCS "07_matrix_keyboard_gpio.c" INCLUDE_DIRS ".")

# Lectura de teclado matricial 4x4 utilizando biblioteca matrix_keyboard (requiere dedic_gpio - NO compatible con ESP32 original)
# idf_component_register(SRCS "08_matrix_keyboard_library.c"
#                        PRIV_REQUIRES matrix_keyboard
#                        INCLUDE_DIRS "")

# Lectura de teclado matricial 4x4 con anti-rebote por software (compatible ESP32) by Cloude Sonnet 4.5
# idf_component_register(SRCS "09_matrix_keyboard_debounce.c" INCLUDE_DIRS ".")

# 10 - BLE Beacon ejemplo. Solo advertising, sin posibilidad de conectarse
# file(GLOB_RECURSE srcs "./10_ble_beacon/main.c" "./10_ble_beacon/src/*.c")
# idf_component_register(SRCS "${srcs}"
#                        PRIV_REQUIRES bt nvs_flash
#                        INCLUDE_DIRS "./10_ble_beacon/include")
# CONFIG_BT_ENABLED=y
# CONFIG_BT_NIMBLE_ENABLED=y
# CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT=n

# 11 - BLE Beacon ejemplo. Solo advertising, sin posibilidad de conectarse
# file(GLOB_RECURSE srcs "./11_ble_connection/main.c" "./11_ble_connection/src/*.c")
# idf_component_register(SRCS "${srcs}"
#                        PRIV_REQUIRES bt nvs_flash
#                        INCLUDE_DIRS "./11_ble_connection/include")
# CONFIG_BT_ENABLED=y
# CONFIG_BT_NIMBLE_ENABLED=y
# CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT=

# 12 BLE con Gatt server
# file(GLOB_RECURSE srcs "./12_ble_gatt_server/main.c" "./12_ble_gatt_server/src/*.c")
# idf_component_register(SRCS "${srcs}"
#                        PRIV_REQUIRES bt nvs_flash esp_driver_gpio
#                        INCLUDE_DIRS "./12_ble_gatt_server/include")
                       # CONFIG_BT_ENABLED=y
# CONFIG_BT_NIMBLE_ENABLED=y
# CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT=
# CONFIG_BLINK_LED_GPIO=y
# CONFIG_BLINK_GPIO=2


# 13 BLE como teclado, mouse o multimedia
set(srcs "./13_ble_hid/esp_hid_device_main.c"
         "./13_ble_hid/esp_hid_gap.c")
set(include_dirs "./13_ble_hid")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                       REQUIRES esp_hid
                       PRIV_REQUIRES nvs_flash)#
# CONFIG_BT_ENABLED=y
# CONFIG_BT_NIMBLE_ENABLED=y
# CONFIG_BT_CONTROLLER_ENABLED=y



