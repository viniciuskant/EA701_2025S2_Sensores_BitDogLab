#include "tcs34725.h"

static uint16_t read_register_16bit(uint8_t reg) {
    uint8_t buffer[2];
    uint8_t command = TCS_COMMAND_BIT | reg;

    i2c_write_blocking(i2c0, SENSOR_ADDR, &command, 1, true);
    i2c_read_blocking(i2c0, SENSOR_ADDR, buffer, 2, false);

    return (buffer[1] << 8) | buffer[0];
}

void tcs34725_init() {
    i2c_init(i2c0, 100 * 1000);

    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    uint8_t enable_cmd[] = {TCS_COMMAND_BIT | TCS_ENABLE_REG, 0x01 | 0x02};
    i2c_write_blocking(i2c0, SENSOR_ADDR, enable_cmd, 2, false);

    sleep_ms(3);
}

void tcs34725_read(color_data_t *data) {
    data->c = read_register_16bit(TCS_CLEAR_REG);
    data->r = read_register_16bit(TCS_RED_REG);
    data->g = read_register_16bit(TCS_GREEN_REG);
    data->b = read_register_16bit(TCS_BLUE_REG);
}