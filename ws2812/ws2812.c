/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define N 21

static inline void put_pixel1(PIO pio, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, 0, pixel_grb << 8u);
}

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

// Fast hsl2rgb algorithm: https://stackoverflow.com/questions/13105185/fast-algorithm-for-rgb-hsl-conversion
uint32_t hsl_to_rgb(uint8_t h, uint8_t s, uint8_t l) {
	if(l == 0) return 0;

	volatile uint8_t  r, g, b, lo, c, x, m;
	volatile uint16_t h1, l1, H;
	l1 = l + 1;
	if (l < 128)    c = ((l1 << 1) * s) >> 8;
	else            c = (512 - (l1 << 1)) * s >> 8;

	H = h * 6;              // 0 to 1535 (actually 1530)
	lo = H & 255;           // Low byte  = primary/secondary color mix
	h1 = lo + 1;

	if ((H & 256) == 0)   x = h1 * c >> 8;          // even sextant, like red to yellow
	else                  x = (256 - h1) * c >> 8;  // odd sextant, like yellow to green

	m = l - (c >> 1);
	switch(H >> 8) {       // High byte = sextant of colorwheel
	 case 0 : r = c; g = x; b = 0; break; // R to Y
	 case 1 : r = x; g = c; b = 0; break; // Y to G
	 case 2 : r = 0; g = c; b = x; break; // G to C
	 case 3 : r = 0; g = x; b = c; break; // C to B
	 case 4 : r = x; g = 0; b = c; break; // B to M
	 default: r = c; g = 0; b = x; break; // M to R
	}

	return (((uint32_t)r + m) << 16) | (((uint32_t)g + m) << 8) | ((uint32_t)b + m);
}

void pattern_rainbow(PIO pio, uint len, uint t){
    const uint8_t angle_difference = 25;
    const uint8_t s=100;
    static uint8_t angle = 0;
    
    for(uint16_t i = 0; i < len /* Change that to your amount of LEDs */; i++) {
        uint32_t rgb_color = hsl_to_rgb(angle + ((i % 256) * angle_difference), s, 127);
        put_pixel1(pio, urgb_u32((rgb_color >> 16) & 0xFF, (rgb_color >> 8) & 0xFF, rgb_color & 0xFF));
        /*
        if(angle % 2){
            if(i%2){
                put_pixel1(pio, urgb_u32(0,0,0));
            } else {
                put_pixel1(pio, urgb_u32((rgb_color >> 16) & 0xFF, (rgb_color >> 8) & 0xFF, rgb_color & 0xFF));
            }
      } else {
            if(!(i%2)){
                put_pixel1(pio, urgb_u32(0,0,0));
            } else {
                put_pixel1(pio, urgb_u32((rgb_color >> 16) & 0xFF, (rgb_color >> 8) & 0xFF, rgb_color & 0xFF));
            }
      }*/
    }
    ++angle;
}
void pattern_snakes(PIO pio, uint len, uint t) {
    for (uint i = 0; i < len; ++i) {
        uint x = (i + (t >> 1)) % 64;
        if (x < 10)
            put_pixel1(pio, urgb_u32(0x5f, 0, 0));
        else if (x >= 15 && x < 25)
            put_pixel1(pio, urgb_u32(0, 0x5f, 0));
        else if (x >= 30 && x < 40)
            put_pixel1(pio, urgb_u32(0, 0, 0x5f));
        else
            put_pixel1(pio, 0);
    }
}

void pattern_random(PIO pio, uint len, uint t) {
    if (t % 8)
        return;
    for (int i = 0; i < len; ++i)
        put_pixel1(pio, rand());
}

void pattern_sparkle(PIO pio, uint len, uint t) {
    if (t % 8)
        return;
    for (int i = 0; i < len; ++i)
        put_pixel1(pio, rand() % 32 ? 0 : 0x5e5e5e);
}

void pattern_greys(PIO pio, uint len, uint t) {
    int max = 10; // let's not draw too much current!
    t %= max;
    for (int i = 0; i < len; ++i) {
        put_pixel1(pio, t * 0x10101);
        if (++t >= max) t = 0;
    }
}
void pattern_vitulak(PIO pio, uint len, uint t) {
    if (t % 20)
        return;
    for (int i = 0; i < len; ++i){
        put_pixel1(pio, rand() / 0x0a0a0a05);
    }
}

typedef void (*pattern)(PIO pio, uint len, uint t);
const struct {
    pattern pat;
    const char *name;
} pattern_table[] = {
        {pattern_rainbow,  "Rainbow"},
        {pattern_snakes,  "Snakes!"},
        {pattern_random,  "Random data"},
        {pattern_sparkle, "Sparkles"},
        {pattern_greys,   "Greys"},
        {pattern_vitulak, "Vitulak zije!"}
};

const int PIN_TX = 15;
const int PIN_TX1 = 16;

int main() {
    //set gpio directions for buttons
    gpio_set_dir(10, GPIO_IN);
    gpio_set_input_enabled(10, true);
    gpio_set_dir(11, GPIO_IN);
    gpio_set_input_enabled(11, true);
    //set gpio pull ups
    gpio_pull_up(10);
    gpio_pull_up(11);

    //set_sys_clock_48();
    stdio_init_all();
    puts("WS2812 Smoke Test");

    // todo get free sm
    PIO pioA = pio0;
    PIO pioB = pio1;
    int sm = 0;
    uint offset = pio_add_program(pioA, &ws2812_program);
    uint offset1 = pio_add_program(pioB, &ws2812_program);

    ws2812_program_init(pioA, sm, offset, PIN_TX, 800000, true);
    ws2812_program_init(pioB, sm, offset1, PIN_TX1, 800000, true);
/*
    for(int i = 0; i < 4; i++){
        put_pixel1(pioB, urgb_u32(0x20, 0x20, 0x00));
    }
*/
    int t = 0;
    int bt = 0;
    int pat=0;
lab001:
    while (1) {
        //int pat = rand() % count_of(pattern_table);
        if(bt==1){
            bt=0;
            pat++; if(pat>=count_of(pattern_table)){
                pat=0;
            }
        }
        if(bt==2){
            bt=0;
            pat--; if(pat<0){
                pat=count_of(pattern_table)-1;
            }
        }
        //count_of(pattern_table);
        int dir = (rand() >> 30) & 1 ? 1 : -1;
        //puts(pattern_table[pat].name);
        //puts(dir == 1 ? "(forward)" : "(backward)");
        for (int i = 0; i < 1000; ++i) {
            pattern_table[pat].pat(pioA, N, t);
            pattern_table[pat].pat(pioB, N, t);
            sleep_ms(10);
            t += dir;
            if(gpio_get(10)==0){
                while(gpio_get(10)==0){
                    bt=1;
                }
                goto lab001;
            }
            if(gpio_get(11)==0){
                while(gpio_get(11)==0){
                    bt=2;
                }
                goto lab001;
            }
        }
        
    }
}