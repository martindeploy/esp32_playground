#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "i2c-lcd.h"


/**
 * Control de LCD 16x2 en modo texto, ejemplo inicial de 
 * (https://controllerstech.com/i2c-in-esp32-esp-idf-lcd-1602) utilizando SPI con ESP32C3 super mini.
 * 
 */


static const char *TAG = "i2c-simple-example";

#define I2C_MASTER_SCL_IO           GPIO_NUM_9       /*!< GPIO number used for I2C master clock  ESP32C3 mini*/
#define I2C_MASTER_SDA_IO           GPIO_NUM_8       /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              I2C_NUM_0                   /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ          400000          /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define SLAVE_ADDRESS_LCD (0x4E >> 1) // 7-bit address: 0x27

char buffer[10];
float num = 12.34;


/**
 * Prototipos de funciones
 */
static void i2c_master_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle);


/**
 * Principal
 */
void app_main(void)
{
   uint8_t data[2];
   i2c_master_bus_handle_t bus_handle;
   i2c_master_dev_handle_t dev_handle;
   i2c_master_init(&bus_handle, &dev_handle);
   ESP_LOGI(TAG, "I2C initialized successfully");

   lcd_init(dev_handle);
   lcd_put_cur(0, 0);
   lcd_send_string("Te quiero");
   lcd_put_cur(1, 0);
   lcd_send_string("Jamoncin");


   //
   //    lcd_put_cur(1, 0);
   //    lcd_send_string("from ESP32");

   // sprintf(buffer, "val=%.2f", num);
   // lcd_put_cur(0, 0);
   // lcd_send_string(buffer);
}

static void i2c_master_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle)
{
   i2c_master_bus_config_t bus_config = {
      .i2c_port = I2C_MASTER_NUM,
      .sda_io_num = I2C_MASTER_SDA_IO,
      .scl_io_num = I2C_MASTER_SCL_IO,
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .glitch_ignore_cnt = 7,
      .flags.enable_internal_pullup = true,
   };
   ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, bus_handle));

   i2c_device_config_t dev_config = {
      .dev_addr_length = I2C_ADDR_BIT_LEN_7,
      .device_address = SLAVE_ADDRESS_LCD,
      .scl_speed_hz = I2C_MASTER_FREQ_HZ,
   };
   ESP_ERROR_CHECK(i2c_master_bus_add_device(*bus_handle, &dev_config, dev_handle));
}