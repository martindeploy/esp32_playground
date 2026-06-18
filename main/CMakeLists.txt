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
idf_component_register(SRCS "06_lcd_gpio.c" INCLUDE_DIRS ".")


