#include "pico/stdlib.h"
#include <keypad.h>
#include <stdio.h>

int cols[4] = {10,11,12,13};
int rows[4] = {22,21,20,19};

void init_keypad(void)
{
    for (int i = 0; i < 4; i++)
    {
        gpio_init(cols[i]);
        gpio_set_dir(cols[i], GPIO_IN);
        gpio_pull_up(cols[i]);
    }
    for (int i = 0; i < 4; i++)
    {
        gpio_init(rows[i]);
        gpio_set_dir(rows[i], GPIO_OUT);
        gpio_put(rows[i], 1);
    }
}

unsigned short get_keypad(void)
{
    unsigned short res = 0;
    for (int i = 0; i < 4; i++)
    {
        gpio_put(rows[i], 0);
        sleep_us(1);
        for (int j = 0; j < 4; j++)
        {
            if(!gpio_get(cols[j]))
            {
                res |= ((1 << j) << (i*4)); 
            } 
        }
        gpio_put(rows[i], 1);
    }
    return res;
}