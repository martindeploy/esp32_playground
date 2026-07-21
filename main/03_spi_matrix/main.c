#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Optputs
#define LED_BOARD   2
#define LED_RED     27
#define LED_GREEN   33

// Configuracion SPI 2 - MAX7219 Matrix LED
#define MAX7219_MOSI_GPIO 13 // DIN
#define MAX7219_MISO_GPIO 12 // not used
#define MAX7219_SCLK      14
#define MAX7219_CS        15

// MAX 7219 registers
#define DECODE_MODE_REG   0x9 // 0
#define INTENSITY_REG     0xA // 7
#define SCAN_LIMIT_REG    0xB // 7
#define SHUTDOWN_REG      0xC // 1
#define DISPLAY_TEST_REG  0xF // 0


static const char *TAG = "03_spi_matrix";


/**
 * Prototipos de funciones
 */
esp_err_t initInputOutputs(void);
esp_err_t initSpi2(void);
void spiWriteMatrix(uint8_t reg, uint8_t value);
void clearMax7219(void);
esp_err_t max7219Init(void);
void drawChessBoard();
void drawChessBoardInverse();




// spiDeviceHandle
spi_device_handle_t spiDeviceHandleMatrix;

// Estado de LEDS
uint8_t ledBoardState = false;
uint8_t ledRedState = false;
uint8_t ledGreenState = false;


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
 * Init SPI2 - Matriz LED
 */
esp_err_t initSpi2(void) {

  ESP_LOGI(TAG, "Iniciando SPI 2 - MAX7219");

  spi_bus_config_t spiBusConfig = {};
  spiBusConfig.mosi_io_num = MAX7219_MOSI_GPIO;
  spiBusConfig.miso_io_num = MAX7219_MISO_GPIO;
  spiBusConfig.sclk_io_num = MAX7219_SCLK;
  spiBusConfig.quadhd_io_num = -1;
  spiBusConfig.quadwp_io_num = -1;
  spiBusConfig.max_transfer_sz = 32;
  spi_bus_initialize(SPI2_HOST, &spiBusConfig, SPI_DMA_CH_AUTO);


  spi_device_interface_config_t spiDeviceInterfaceConfig = {};
  spiDeviceInterfaceConfig.mode = 0;
  spiDeviceInterfaceConfig.duty_cycle_pos = 120;
  spiDeviceInterfaceConfig.clock_speed_hz = 1000000;
  spiDeviceInterfaceConfig.spics_io_num = MAX7219_CS;
  spiDeviceInterfaceConfig.flags = SPI_DEVICE_HALFDUPLEX;
  spiDeviceInterfaceConfig.queue_size = 1;
  spiDeviceInterfaceConfig.pre_cb = NULL;
  spiDeviceInterfaceConfig.post_cb = NULL;

  spi_bus_add_device(SPI2_HOST, &spiDeviceInterfaceConfig, &spiDeviceHandleMatrix);

  return ESP_OK;
}



/**
 * Inicialilzar matrix LED
 */
esp_err_t max7219Init(void) {
  spiWriteMatrix(DECODE_MODE_REG, 0);
  spiWriteMatrix(INTENSITY_REG, 7);
  spiWriteMatrix(SCAN_LIMIT_REG, 7);
  spiWriteMatrix(SHUTDOWN_REG, 1);
  spiWriteMatrix(DISPLAY_TEST_REG, 0);
  return ESP_OK;
}

/**
 * Clear all LEDS
 */
void clearMax7219(void){
  ESP_LOGI(TAG, "Cleaning all LEDs...");

  for (size_t i = 0; i < 8; i++)
  {
    spiWriteMatrix(i+1, 0);
  }
  
}

/**
 * 
 */
void spiWriteMatrix(uint8_t reg, uint8_t value) {
  uint8_t data[2] = {reg, value};
  spi_transaction_t spiTransaction = {
    .tx_buffer = data,
    .length = 16
  };
  spi_device_transmit(spiDeviceHandleMatrix, &spiTransaction);
}



void app_main(void)
{
  ESP_ERROR_CHECK(initSpi2());
  ESP_ERROR_CHECK(max7219Init());
  clearMax7219();
  
  gpio_dump_io_configuration(stdout, SOC_GPIO_VALID_GPIO_MASK);
  while (true)
  {
    spiWriteMatrix(INTENSITY_REG, 7);
    drawChessBoard();
    vTaskDelay(pdMS_TO_TICKS(1000)); 
    spiWriteMatrix(INTENSITY_REG, 1);
    drawChessBoardInverse();
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Test display
    spiWriteMatrix(DISPLAY_TEST_REG, 1); // Pasar a Display Test Mode (sete intensidad maxima)
    vTaskDelay(pdMS_TO_TICKS(1000)); 
    spiWriteMatrix(DISPLAY_TEST_REG, 0); // Volver a Normal operation


  }
  
}

/**
 * Tablero de ajedrez
 */
void drawChessBoard() {
  // Tablero de ajedrez
  spiWriteMatrix(1, 0b01010101);
  spiWriteMatrix(2, 0b10101010);
  spiWriteMatrix(3, 0b01010101);
  spiWriteMatrix(4, 0b10101010);
  spiWriteMatrix(5, 0b01010101);
  spiWriteMatrix(6, 0b10101010);
  spiWriteMatrix(7, 0b01010101);
  spiWriteMatrix(8, 0b10101010);
}

/**
 * Tablero de ajedrez inversa
 */
void drawChessBoardInverse() {
  // Tablero de ajedrez
  spiWriteMatrix(1, 0b10101010);
  spiWriteMatrix(2, 0b01010101);
  spiWriteMatrix(3, 0b10101010);
  spiWriteMatrix(4, 0b01010101);
  spiWriteMatrix(5, 0b10101010);
  spiWriteMatrix(6, 0b01010101);
  spiWriteMatrix(7, 0b10101010);
  spiWriteMatrix(8, 0b01010101);
}