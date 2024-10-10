#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <display_ctrl.h>


uint8_t display_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8] = {0};

void send_command(uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd};  // 0x00 indicates command
    int ret = i2c_write_blocking(I2C_PORT, DISPLAY_ADDRESS, buf, 2, true);
}

void send_data(uint8_t data) {
    uint8_t buf[2] = {0x40, data};  // 0x40 indicates data
    int ret = i2c_write_blocking(I2C_PORT, DISPLAY_ADDRESS, buf, 2, true);
}

void clear_display() {
    memset(display_buffer, 0, sizeof(display_buffer));
}

void draw_pixel(int x, int y, bool on) {
    if (x < 0 || x >= DISPLAY_WIDTH || y < 0 || y >= DISPLAY_HEIGHT) return;
    int byte_index = x + (y / 8) * DISPLAY_WIDTH;
    int bit_index = y % 8;
    if (on) {
        display_buffer[byte_index] |= (1 << bit_index);
    } else {
        display_buffer[byte_index] &= ~(1 << bit_index);
    }
}

void update_display() {
    for (int page = 0; page < DISPLAY_HEIGHT/8; page++) {
        send_command(ST7567S_SET_PAGE_ADDRESS | page);
        send_command(ST7567S_SET_COLUMN_ADDRESS_UPPER | 0);
        send_command(ST7567S_SET_COLUMN_ADDRESS_LOWER | 0);
        for (int col = 0; col < DISPLAY_WIDTH; col++) {
            send_data(display_buffer[page * DISPLAY_WIDTH + col]);
        }
    }
}

void clear_full_display(){
    for (int page = 0; page < 8; page++) {
        send_command(ST7567S_SET_PAGE_ADDRESS | page);
        send_command(ST7567S_SET_COLUMN_ADDRESS_UPPER | 0);
        send_command(ST7567S_SET_COLUMN_ADDRESS_LOWER | 0);
        for (int col = 0; col < 128; col++) {
            send_data(0);
        }
    }
}

void init_display() {
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    send_command(ST7567S_BIAS_1_9);  // Set bias to 1/9
    send_command(ST7567S_SEG_DIRECTION_NORMAL);  // ADC select (SEG output direction)
    send_command(ST7567S_COM_DIRECTION_REVERSE);  // SHL select (COM output direction)
    send_command(ST7567S_INVERSE_DISPLAY_OFF);  // Display normal (not inverted)
    send_command(ST7567S_SET_START_LINE | 0x00);  // Set start line to 0
    send_command(ST7567S_SET_POWER_CONTROL | 0x07);  // Power control set (Booster, Regulator, and Follower on)
    send_command(ST7567S_SET_RESISTOR_RATIO | 0x05);  // Resistor ratio set (5)
    
    // Set electronic volume (contrast)
    send_command(ST7567S_ELECTRONIC_VOLUME_MODE_SET);
    send_command(0x20);  // Value from 0 to 63 (0x3F)

    send_command(ST7567S_ALL_PIXEL_NORMAL);  // All pixels normal
    send_command(ST7567S_DISPLAY_ON);  // Display on
}