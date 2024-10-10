#include <display_ctrl.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include <keypad.h>
#include <chip8.h>
#include <rom.h>

void get_keys(unsigned char keys[]);
void draw_to_screen(unsigned char display_buf[]);

unsigned char keymap[16] = {0x1, 0x2, 0x3, 0xC, 
                            0x4, 0x5, 0x6, 0xD,
                            0x7, 0x8, 0x9, 0xE, 
                            0xA, 0x0, 0xB, 0xF};

int main()
{
    stdio_init_all();
    init_display();
    init_keypad();

    int should_quit = 0;

    struct chip8 *c8 = malloc(sizeof(struct chip8));
    memset(c8, 0, sizeof(struct chip8));
    clear_full_display();

    memcpy(c8->memory, rom_data, rom_data_length);

    init(c8);

    while (should_quit != -1)
    {
        should_quit = cycle(c8);
        get_keys(c8->keys);

        if(should_quit == DRAW_FLAG)
        {
            draw_to_screen(c8->display_buf);
        }

        usleep(DELAY_US);
    }

    free(c8);
    while(0)
    {
        sleep_ms(100);
    }
}

void get_keys(unsigned char keys[])
{
    unsigned short keypad = get_keypad();
    for(int i = 0; i < 16; i++)
    {
       keys[keymap[i]] = (keypad >> i) & 0x1;
    }
}

void draw_to_screen(unsigned char display_buf[])
{
    clear_display();
    for(int i = 0; i < VIDEO_WIDTH; i++)
    {
        for (int j = 0; j < VIDEO_HEIGHT; j++)
        {
            draw_pixel(i,j,display_buf[j*VIDEO_WIDTH + i]);
        }
    }
    update_display();
}