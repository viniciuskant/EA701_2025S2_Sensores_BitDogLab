#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define SENSOR_ADDR       0x29
#define TCS_COMMAND_BIT   0x80
#define TCS_ENABLE_REG    0x00
#define TCS_CLEAR_REG     0x14
#define TCS_RED_REG       0x16
#define TCS_GREEN_REG     0x18
#define TCS_BLUE_REG      0x1A

#define I2C_SCL_PIN 1
#define I2C_SDA_PIN 0

typedef struct {
    uint16_t r, g, b, c;
} color_data_t;

void tcs34725_read(color_data_t *data);
void tcs34725_init();

#endif 