#ifndef _DISPLAY_CTRL_H
#define _DISPLAY_CTRL_H

#include "hardware/i2c.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5

#define DISPLAY_ADDRESS 0x3F  // Typical I2C address for ST7567S, but verify this
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

// ST7567S Commands
#define ST7567S_DISPLAY_OFF 0xAE
#define ST7567S_DISPLAY_ON 0xAF
#define ST7567S_SET_START_LINE 0x40
#define ST7567S_SET_PAGE_ADDRESS 0xB0
#define ST7567S_SET_COLUMN_ADDRESS_UPPER 0x10
#define ST7567S_SET_COLUMN_ADDRESS_LOWER 0x00
#define ST7567S_SEG_DIRECTION_NORMAL 0xA0
#define ST7567S_SEG_DIRECTION_REVERSE 0xA1
#define ST7567S_COM_DIRECTION_NORMAL 0xC0
#define ST7567S_COM_DIRECTION_REVERSE 0xC8
#define ST7567S_INVERSE_DISPLAY_OFF 0xA6
#define ST7567S_INVERSE_DISPLAY_ON 0xA7
#define ST7567S_ALL_PIXEL_ON 0xA5
#define ST7567S_ALL_PIXEL_NORMAL 0xA4
#define ST7567S_BIAS_1_9 0xA2
#define ST7567S_BIAS_1_7 0xA3
#define ST7567S_RESET 0xE2
#define ST7567S_SET_POWER_CONTROL 0x28
#define ST7567S_SET_RESISTOR_RATIO 0x20
#define ST7567S_ELECTRONIC_VOLUME_MODE_SET 0x81

void init_display(void);

void clear_display(void);

void update_display(void);

void draw_pixel(int x, int y, bool on);

void clear_full_display(void);

#endif