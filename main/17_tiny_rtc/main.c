#include <time.h>
#include <sys/time.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ds1307.h>
#include <string.h>
#include <driver/i2c_master.h>
#include "esp_log.h"



/**
 * Comentarios
 * DS1307: Cristal externo, requiere modificacion para anular circuito de carga de bateria si
 * se utiliza con con CR2032 (no recargable)
 * DS3231: Sensor de temperatura, no probado
 */


/**
 * Ejemplo basado en https://github.com/esp-idf-lib/ds1307/blob/main/examples/default/main/main.c
 * 
 */


#define SLAVE_ADDRESS_RTC 0x68
#define I2C_MASTER_SCL_IO           GPIO_NUM_9       /*!< GPIO number used for I2C master clock  ESP32C3 mini*/
#define I2C_MASTER_SDA_IO           GPIO_NUM_8       /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              I2C_NUM_0                   /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ          400000          /*!< I2C master clock frequency */
#define I2C_MASTER_TIMEOUT_MS       1000

static const char *TAG = "Tiny RTC read/write";


void ds1307_test(void *pvParameters)
{
    i2c_dev_t dev;
    memset(&dev, 0, sizeof(i2c_dev_t));

    ESP_ERROR_CHECK(ds1307_init_desc(&dev, I2C_MASTER_NUM, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO));
    
    // En la biblioteca tiene 400kHz
    dev.cfg.master.clk_speed = 100000; // DS1307 soporta maximo 100kHz (standard mode)

    // setup datetime: 2026-07-10 00:52:10
    // Nota: el DS1307 solo soporta años 2000-2099 (tm_year debe ser 100-199)
    struct tm time =
    {
        .tm_year = 2026 - 1900,  // = 126
        .tm_mon  = 6,  // 0-based (julio = 6)
        .tm_mday = 10,
        .tm_hour = 23,
        .tm_min  = 9,
        .tm_sec  = 0
    };
    // Descomentar esta linea para escribir la hora en el modulo
    // ESP_ERROR_CHECK(ds1307_set_time(&dev, &time));

    while (1)
    {
        ds1307_get_time(&dev, &time);

        printf("%04d-%02d-%02d %02d:%02d:%02d\n", time.tm_year + 1900 /*Add 1900 for better readability*/, time.tm_mon + 1,
               time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main()
{
    ESP_ERROR_CHECK(i2cdev_init());

    xTaskCreate(ds1307_test, "ds1307_test", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);
}