#include <stdio.h>
#include <math.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define PIN_NUM_MISO 19   // SO
#define PIN_NUM_CLK  18   // SCK
#define PIN_NUM_CS    2   // CS

spi_device_handle_t max31855;

static void max31855_init(void) {
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = -1,         // not used
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4,
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1000000,   // 1 MHz (safe for MAX31855)
        .mode = 0,                   // SPI mode 0
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 1,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &max31855));
}

static int32_t max31855_read_raw(void) {
    uint8_t rx_buf[4] = {0};
    spi_transaction_t t = {
        .length = 32,        // 32 bits
        .rx_buffer = rx_buf,
    };

    esp_err_t ret = spi_device_transmit(max31855, &t);
    if (ret != ESP_OK) {
        return -1;
    }

    return (rx_buf[0] << 24) | (rx_buf[1] << 16) | (rx_buf[2] << 8) | rx_buf[3];
}

static float max31855_read_celsius(void) {
    int32_t v = max31855_read_raw();
    if (v < 0) return NAN;

    // Check fault bits
    if (v & 0x7) {
        return NAN;
    }

    // Extract temperature (bits 31..18)
    int16_t tempData = (v >> 18) & 0x3FFF;

    // Sign extend if negative
    if (tempData & 0x2000) {
        tempData |= 0xC000;
    }

    return tempData * 0.25f; // Each LSB = 0.25 °C
}

void app_main(void) {
    max31855_init();

    while (1) {
        float tempC = max31855_read_celsius();
        if (!isnan(tempC)) {
            // Print just the value for easy parsing by another program
            printf("%.2f\n", tempC);
        } else {
            printf("NaN\n");  // error reading
        }
        fflush(stdout); // ensure it gets pushed out immediately
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 Hz update
    }
}
