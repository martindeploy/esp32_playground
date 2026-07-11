#include <time.h>
#include <sys/time.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/i2c_master.h>
#include "esp_log.h"



/**
 * DS1307: Cristal externo, requiere modificacion para anular circuito de carga de bateria si
 * se utiliza con con CR2032 (no recargable)
 * DS3231: Sensor de temperatura, no probado
 */


// Prototipos
static void i2c_master_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle);
void config_sytem_time(int anio, int mes, int dia, int hora, int min, int seg);
void read_system_time();
void read_rtc_time();
void write_rtc_time();
uint8_t dec_to_bcd(uint8_t val);
uint8_t bcd_to_dec(uint8_t val);


#define SLAVE_ADDRESS_RTC (0x68 >> 1)
#define I2C_MASTER_SCL_IO           GPIO_NUM_9       /*!< GPIO number used for I2C master clock  ESP32C3 mini*/
#define I2C_MASTER_SDA_IO           GPIO_NUM_8       /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              I2C_NUM_0                   /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ          400000          /*!< I2C master clock frequency */
#define I2C_MASTER_TIMEOUT_MS       1000

static const char *TAG = "Tiny RTC read/write";


i2c_master_bus_handle_t bus_handle;
i2c_master_dev_handle_t dev_handle;


void app_main(void)
{
    // config_sytem_time(1987, 5, 20, 5, 30, 0);
    i2c_master_init(&bus_handle, &dev_handle);
    
    write_rtc_time();
    vTaskDelay(pdMS_TO_TICKS(3000));
    ESP_LOGI(TAG, "Waiting...");


    read_rtc_time();

    while (1)
    {
        read_system_time();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}

void write_rtc_time() 
{
    // 1. Lees el primer byte (registro 0x00 - Segundos)
    uint8_t reg_segundos = 0x00;
    uint8_t dato_segundos;
    i2c_master_transmit_receive(dev_handle, &reg_segundos, 1, &dato_segundos, 1, -1);

    // 2. Verificas si el bit 7 (CH) está activo (0x80 es 10000000 en binario)
    if (dato_segundos & 0x80) {
        ESP_LOGW("RTC", "¡El DS1307 está detenido! Configurando fecha base...");

        // 3. Preparas un buffer con una fecha base inventada por ti (ej: 1 de Enero de 2026, 00:00:00)
        // Se debe enviar en formato BCD y el primer byte pone el bit CH en 0
        uint8_t nueva_fecha[8] = {
            0x00,       // Registro inicial: 0x00
            0x00,       // Segundos: 00 (y bit CH en 0 para que empiece a correr)
            0x00,       // Minutos: 00
            0x00,       // Horas: 00 (Modo 24h)
            0x01,       // Día de la semana (1-7)
            0x01,       // Día del mes: 01
            0x01,       // Mes: 01 (Enero)
            0x26        // Año: 26 (Representa el año 2026)
        };

        // 4. Escribes los datos para arrancar el reloj
        i2c_master_transmit(dev_handle, nueva_fecha, 8, -1);
    } else {
        ESP_LOGI("RTC", "El reloj ya está funcionando con la hora de la batería.");
    }
}


void read_rtc_time()
{
    // Registrar 0x00 es donde empiezan los segundos en el DS1307
    uint8_t reg_addr = 0x00; 
    uint8_t data_rtc[7]; // Array para almacenar seg, min, hora, dia_semana, dia_mes, mes, año

    // Transacción I2C: Escribir la dirección del registro y leer los 7 bytes de tiempo
    i2c_master_transmit_receive(dev_handle, &reg_addr, 1, data_rtc, 7, -1);

    // Decodificar (Quitando el bit de CH 'Clock Halt' en los segundos si fuera necesario)
    int segundos = bcd_to_dec(data_rtc[0] & 0x7F);
    int minutos  = bcd_to_dec(data_rtc[1]);
    int horas    = bcd_to_dec(data_rtc[2] & 0x3F); // Modo 24h
    int dia      = bcd_to_dec(data_rtc[4]);
    int mes      = bcd_to_dec(data_rtc[5]);
    int anio     = bcd_to_dec(data_rtc[6]) + 2000;

    // Finalmente, pasas estos datos a la función del paso 2
    config_sytem_time(anio, mes, dia, horas, minutos, segundos);

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
        .device_address = SLAVE_ADDRESS_RTC,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(*bus_handle, &dev_config, dev_handle));
    
    ESP_LOGI(TAG, "I2C initialized successfully");

}



void config_sytem_time(int anio, int mes, int dia, int hora, int min, int seg) {
    struct tm t;
    t.tm_year = anio - 1900;  // El año cuenta desde 1900
    t.tm_mon = mes - 1;       // Los meses van de 0 a 11
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;          // No usar horario de verano automático

    // Convertir struct tm a tiempo Unix (segundos transcurridos desde 1970)
    time_t time_unix = mktime(&t);

    // Aplicar el tiempo Unix al sistema del ESP32
    struct timeval tv = {
        .tv_sec = time_unix,
        .tv_usec = 0
    };
    settimeofday(&tv, NULL);
    
    ESP_LOGI("RELOJ", "Hora del sistema actualizada correctamente.");
}

void read_system_time() {
    time_t ahora;
    struct tm info_tiempo;
    char buffer_texto[64];

    // Obtener el tiempo Unix actual
    time(&ahora);
    
    // Convertir el tiempo Unix a la estructura local tm
    localtime_r(&ahora, &info_tiempo);

    // Formatear el texto de manera elegante (Ejemplo: 2026-07-10 20:45:00)
    strftime(buffer_texto, sizeof(buffer_texto), "%c", &info_tiempo);
    
    ESP_LOGI("RELOJ", "La hora actual es: %s", buffer_texto);
}


uint8_t dec_to_bcd(uint8_t val) 
{ 
    return ((val / 10) << 4) + (val % 10); 
}


uint8_t bcd_to_dec(uint8_t val) 
{ 
    return ((val >> 4) * 10) + (val & 0x0F); 
}
